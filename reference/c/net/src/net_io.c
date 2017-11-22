#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Net.h>

/*
 * Efl.IO Networked examples.
 *
 * This example builds on the core_io example by connecting to a remote server
 * using a dialler and a command queue.The response is printed to stdout.
 */

static Eina_List *_waiting = NULL;
static Eina_List *_commands = NULL;
static Eina_Slice _delimiter;
static Eo *_dialer = NULL, *_sender = NULL, *_receiver = NULL;

static void
_command_next(void)
{
   Eina_Slice slice;
   char *cmd;
   Eo *send_queue;

   send_queue = efl_io_copier_source_get(_sender);
   if (!_commands)
     {
        efl_io_queue_eos_mark(send_queue);
        return;
     }

   cmd = _commands->data;
   _commands = eina_list_remove_list(_commands, _commands);

   slice = (Eina_Slice)EINA_SLICE_STR(cmd);
   efl_io_writer_write(send_queue, &slice, NULL);
   fprintf(stderr, "INFO: sent '" EINA_SLICE_STR_FMT "'\n",
           EINA_SLICE_STR_PRINT(slice));

   /* don't use _delimiter directly, 'len' may be changed! */
   slice = _delimiter;
   efl_io_writer_write(send_queue, &slice, NULL);
}

static void
_receiver_data(void *data EINA_UNUSED, const Efl_Event *event)
{
   Eina_Slice slice = efl_io_queue_slice_get(event->object);

   // Can be caused when we issue efl_io_queue_clear()
   if (slice.len == 0) return;

   /*
    * If the server didn't send us the line terminator and closed the
    * connection (ie: efl_io_reader_eos_get() == true) or if the buffer
    * limit was reached then we may have a line without a trailing delimiter.
    */
   if (eina_slice_endswith(slice, _delimiter))
     slice.len -= _delimiter.len;

   fprintf(stderr, "INFO: received '" EINA_SLICE_STR_FMT "'\n",
           EINA_SLICE_STR_PRINT(slice));

   efl_io_queue_clear(event->object);
   _command_next();
}

static void
_dialer_connected(void *data EINA_UNUSED, const Efl_Event *event)
{
   fprintf(stderr, "INFO: connected to %s (%s)\n",
           efl_net_dialer_address_dial_get(event->object),
           efl_net_socket_address_remote_get(event->object));

   _command_next();
}

static void
_copier_done(void *data EINA_UNUSED, const Efl_Event *event)
{
   fprintf(stderr, "INFO: %s done\n", efl_name_get(event->object));

   _waiting = eina_list_remove(_waiting, event->object);
   if (!_waiting)
     efl_exit(EXIT_SUCCESS);
}

static void
_copier_error(void *data EINA_UNUSED, const Efl_Event *event)
{
   const Eina_Error *perr = event->info;

   fprintf(stderr, "INFO: %s error: #%d '%s'\n",
           efl_name_get(event->object), *perr, eina_error_msg_get(*perr));

   efl_exit(EXIT_FAILURE);
}

EFL_CALLBACKS_ARRAY_DEFINE(copier_cbs,
                           { EFL_IO_COPIER_EVENT_DONE, _copier_done },
                           { EFL_IO_COPIER_EVENT_ERROR, _copier_error });

EAPI void
efl_pause(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
}

EAPI void
efl_resume(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
}

EAPI void
efl_terminate(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   if (_waiting)
     {
        fprintf(stderr, "ERROR: %d operations were waiting!\n",
                eina_list_count(_waiting));
        eina_list_free(_waiting);
        _waiting = NULL;
     }

   if (_receiver)
     {
        efl_io_closer_close(_receiver);
        efl_del(efl_io_copier_destination_get(_receiver));
        efl_del(_receiver);
     }

   if (_sender)
     {
        efl_io_closer_close(_sender);
        efl_del(efl_io_copier_source_get(_sender));
        efl_del(_sender);
     }

   if (_dialer)
     efl_del(_dialer);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   char *address = "example.com:80";
   unsigned long buffer_limit = 128;
   Eina_Error err;
   Eo *loop, *send_queue, *receive_queue;

   _commands = eina_list_append(_commands, "HEAD / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n");
   _delimiter = (Eina_Slice)EINA_SLICE_STR("\r\n");

   /*
    * Without a send_queue we'd have to manually implement an
    * Efl_Io_Reader object that would provide partial data when
    * Efl_Io_Reader.read() is called by Efl_Io_Copier. This is
    * cumbersome... we just want to write a full command and have the
    * queue to handle that for us.
    *
    * Our example's usage is to write each command at once followed by
    * the line_delimiter, then wait for a reply from the server, then
    * write another.
    */
   send_queue = efl_add(EFL_IO_QUEUE_CLASS, NULL,
                        efl_name_set(efl_added, "send_queue"),
                        efl_io_queue_limit_set(efl_added, buffer_limit));
   if (!send_queue)
     {
        fprintf(stderr, "ERROR: could not create Efl_Io_Queue (send)\n");
        efl_exit(EXIT_FAILURE);
     }

   /*
    * Without a receive_queue we'd have to manually implement an
    * Efl_Io_Writer object that would handle write of partial data
    * with Efl_Io_Writer.write() is called by Efl_Io_Copier.
    *
    * For output we could have another solution as well: use NULL
    * destination and handle "line" or "data" events manually,
    * stealing the buffer so it doesn't grow.
    *
    * Our example's usage is to peek its data with slice_get() then
    * clear().
    */
   receive_queue = efl_add(EFL_IO_QUEUE_CLASS, NULL,
                           efl_name_set(efl_added, "receive_queue"),
                           efl_io_queue_limit_set(efl_added, buffer_limit),
                           efl_event_callback_add(efl_added, EFL_IO_QUEUE_EVENT_SLICE_CHANGED, _receiver_data, NULL));
   if (!receive_queue)
     {
        fprintf(stderr, "ERROR: could not create Efl_Io_Queue (receive)\n");
        efl_exit(EXIT_FAILURE);
     }

   /*
    * From here on it's mostly the same all Efl_Io_Copier would do,
    * check efl_io_copier_simple_example.c and efl_io_copier_example.c
    */

   /*
    * some objects such as the Efl.Io.Copier and Efl.Net.Dialer.Tcp
    * depend on main loop, thus their parent must be a loop
    * provider. We use the loop itself.
    */
   loop = ecore_main_loop_get();

   /* The TCP client to use to send/receive network data */
   _dialer = efl_add(EFL_NET_DIALER_TCP_CLASS, loop,
                    efl_name_set(efl_added, "dialer"),
                    efl_event_callback_add(efl_added, EFL_NET_DIALER_EVENT_CONNECTED, _dialer_connected, NULL));
   if (!_dialer)
     {
        fprintf(stderr, "ERROR: could not create Efl_Net_Dialer_Tcp\n");
        efl_exit(EXIT_FAILURE);
     }

   /* sender: send_queue->network */
   _sender = efl_add(EFL_IO_COPIER_CLASS, loop,
                    efl_name_set(efl_added, "sender"),
                    efl_io_copier_line_delimiter_set(efl_added, _delimiter),
                    efl_io_copier_source_set(efl_added, send_queue),
                    efl_io_copier_destination_set(efl_added, _dialer),
                    efl_event_callback_array_add(efl_added, copier_cbs(), NULL));
   if (!_sender)
     {
        fprintf(stderr, "ERROR: could not create Efl_Io_Copier (sender)\n");
        efl_exit(EXIT_FAILURE);
     }

   /* receiver: network->receive_queue */
   _receiver = efl_add(EFL_IO_COPIER_CLASS, loop,
                      efl_name_set(efl_added, "receiver"),
                      efl_io_copier_line_delimiter_set(efl_added, _delimiter),
                      efl_io_copier_source_set(efl_added, _dialer),
                      efl_io_copier_destination_set(efl_added, receive_queue),
                      efl_event_callback_array_add(efl_added, copier_cbs(), NULL));
   if (!_receiver)
     {
        fprintf(stderr, "ERROR: could not create Efl_Io_Copier (receiver)\n");
        efl_exit(EXIT_FAILURE);
     }

   err = efl_net_dialer_dial(_dialer, address);
   if (err)
     {
        fprintf(stderr, "ERROR: could not dial %s: %s\n",
                address, eina_error_msg_get(err));
        efl_exit(EXIT_FAILURE);
     }

   _waiting = eina_list_append(_waiting, _sender);
   _waiting = eina_list_append(_waiting, _receiver);
}
EFL_MAIN_EX()

