#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

/* 
 * These are some helper methods for triggering lifecycle events for the purpose of this demo.
 * efl_pause and efl_resume may never be called for your application, depending on your environment.
 * This demo triggers them directly to show how you can respond.
 */
static void
_method_delay_call(Efl_Event_Cb method, double delay, Efl_Loop *loop)
{
   efl_add(EFL_LOOP_TIMER_CLASS, loop, 
           efl_event_callback_add(efl_added, EFL_LOOP_TIMER_EVENT_TICK, method, loop),
           efl_loop_timer_interval_set(efl_added, delay));
}

static void
_simulate_app_exit(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   efl_exit(0);
}

static void
_simulate_app_foreground(void *data, const Efl_Event *ev)
{
   Efl_Loop *loop = data;
   efl_event_callback_call(loop, EFL_LOOP_EVENT_RESUME, NULL);
   efl_del(ev->object);

   _method_delay_call(_simulate_app_exit, 0.5, loop);
}

static void
_simulate_app_background(void *data, const Efl_Event *ev)
{
   Efl_Loop *loop = data;
   efl_event_callback_call(loop, EFL_LOOP_EVENT_PAUSE, NULL);
   efl_del(ev->object);

   _method_delay_call(_simulate_app_foreground, 1.0, loop);
}
/* end helper methods - what follows below is efl event handling that you can use in your apps. */

EAPI void
efl_pause(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   printf("Lifecycle: paused\n");
}

EAPI void
efl_resume(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   printf("Lifecycle: resumed\n");
}
 
EAPI void
efl_terminate(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   printf("Lifecycle: terminated\n");
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   printf("Lifecycle: launched\n");

   // here we trigger the chain of simulated events to show how an app could respond to system lifecycle.
   _method_delay_call(_simulate_app_background, 0.5, ev->object);
}
EFL_MAIN_EX()

