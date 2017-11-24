#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

#define FILENAME "/tmp/core_loop_test.txt"

/*
 * Efl Core Loop examples.
 *
 * A collection of loop examples. First we print out loop information and
 * show how loops are inherited.
 *
 * Then we move on to the Efl.Loop.Fd and show how you could use a loop to
 * read or write to an FD without blocking.
 */

static void _read_loop_create();

static void
_print_loop(Efl_Loop *loop, const char *label)
{
   printf("%s loop found at %p \"%s\"\n", label, loop, efl_name_get(loop));
}

static void
_loop_fd_read_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop_Fd *loop_fd;
   char buf[7];
   int len;

   loop_fd = event->object;

   len = read(efl_loop_fd_file_get(loop_fd), &buf, sizeof(buf));

   // here we are exiting
   if (len <= 0)
     {
        efl_del(loop_fd);
        unlink(FILENAME);

        efl_exit(0);
        return;
     }

   buf[len] = 0;
   _print_loop(loop_fd, "Reading from");
}

static void
_loop_fd_write_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop_Fd *loop_fd;
   static int _count = 0;

   loop_fd = event->object;

   // we have outputted all we want to, remove the write handler
   // start checking for read availability instead
   if (_count >= 5)
     {
        efl_del(loop_fd);

        _read_loop_create();
        return;
     }

   _count++;
   _print_loop(loop_fd, "Writing from");
   write(efl_loop_fd_file_get(loop_fd), eina_slstr_printf("TEST %d\n", _count), 7);
}

static void
_write_loop_create()
{
   Efl_Loop_Fd *loop_fd;
   FILE *file;
   int fd;

   loop_fd = efl_add(EFL_LOOP_FD_CLASS, NULL,
                     efl_name_set(efl_added, "Write Loop"));

   efl_event_callback_add(loop_fd, EFL_LOOP_FD_EVENT_WRITE, _loop_fd_write_cb, NULL);

   file = fopen(FILENAME, "w+");
   fd = fileno(file);

   printf("Opened file %s with fd %d\n", FILENAME, fd);
   efl_loop_fd_file_set(loop_fd, fd);
}

static void
_read_loop_create()
{
   Efl_Loop_Fd *loop_fd;
   FILE *file;
   int fd;

   loop_fd = efl_add(EFL_LOOP_FD_CLASS, NULL,
                     efl_name_set(efl_added, "Read Loop"));

   efl_event_callback_add(loop_fd, EFL_LOOP_FD_EVENT_READ, _loop_fd_read_cb, NULL);

   file = fopen(FILENAME, "r");
   fd = fileno(file);

   printf("Opened file %s with fd %d\n", FILENAME, fd);
   efl_loop_fd_file_set(loop_fd, fd);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   const Efl_Version *version;
   Efl_Loop *loop;

   version = efl_loop_efl_version_get(ev->object);
   printf("Running on EFL version %d.%d.%d [%s]\n\n", version->major, version->minor,
                                                      version->micro, version->build_id);

   loop = ev->object;
   _print_loop(loop, "Current");

   loop = efl_loop_main_get(loop);
   _print_loop(loop, "Application Main");

   loop = efl_add(EFL_LOOP_USER_CLASS, loop,
                  efl_name_set(efl_added, "User Loop"));
   _print_loop(loop, "Child");
   efl_del(loop);
   printf("\n");

   _write_loop_create();
   // we will call _read_loop_create() once write is complete!

   // we will exit from the end of the read loop
}
EFL_MAIN()

