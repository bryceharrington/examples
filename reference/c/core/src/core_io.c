#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Efl.IO examples.
 *
 * This IO example simply copies data from a source (stdin) to a
 * destination (stdout) using an Efl.Io.Copier.
 */

Eo *_copier = NULL;

static void
_copier_done(void *data EINA_UNUSED, const Efl_Event *event)
{
   fprintf(stderr, "INFO: %s done\n", efl_name_get(event->object));

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
   if (!_copier)
     return;

   efl_del(efl_io_copier_source_get(_copier));
   efl_del(efl_io_copier_destination_get(_copier));
   efl_del(_copier);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   Eo *input, *output, *loop;

   /*
    * some objects such as the Efl.Io.Copier depend on main loop,
    * thus their parent must be a loop provider. We use the loop itself.
    */
   loop = ev->object;

   // set up our objects to copy, use stdin and stdout
   input = efl_add(EFL_IO_STDIN_CLASS, loop);
   output = efl_add(EFL_IO_STDOUT_CLASS, loop);

   // copier: set up a copied from input to output
   _copier = efl_add(EFL_IO_COPIER_CLASS, loop,
                     efl_name_set(efl_added, "Copier"),
                     efl_io_copier_source_set(efl_added, input),
                     efl_io_copier_destination_set(efl_added, output),
                     efl_event_callback_array_add(efl_added, copier_cbs(), NULL));
   if (!_copier)
     {
        fprintf(stderr, "ERROR: could not create Efl_Io_Copier\n");
        efl_exit(EXIT_FAILURE);
     }

   printf("Type something here and press enter, it will be copied to stdout...\n");
   printf("  (press Ctrl-D to exit)\n");
}
EFL_MAIN_EX()

