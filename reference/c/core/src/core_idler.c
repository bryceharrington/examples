#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Efl Core Idler examples.
 *
 * Here we register callbacks to execute code when the loop is idle.
 * We also record when we enter or exit the idle state.
 *
 * We initiate a timer to exit the idle state and then exit the application
 */

Efl_Loop_Timer *_timer;

// the idler enter callback
static void
_enter_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("IDLE ENTER: Entering idle state.\n");
}

// the idler exit callback
static void
_exit_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("IDLE EXIT: Exiting idle state.\n");
}

// the idler callback - ran while the loop is idle
static void
_idler_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("IDLER: executing idler callback while in idle state.\n");
}

// Our timer callback ticks at the specified interval to interrupt the idle state
static void
_timer_cb(void *data, const Efl_Event *event)
{
   printf("TIMER: timer callback called, exiting.\n");

   efl_del(_timer);
   efl_exit(0);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *loop = ev->object;

   efl_event_callback_add(loop, EFL_LOOP_EVENT_IDLE, _idler_cb, NULL);
   efl_event_callback_add(loop, EFL_LOOP_EVENT_IDLE_ENTER, _enter_cb, NULL);
   efl_event_callback_add(loop, EFL_LOOP_EVENT_IDLE_EXIT, _exit_cb, NULL);

   _timer = efl_add(EFL_LOOP_TIMER_CLASS, loop,
                    efl_loop_timer_interval_set(efl_added, 0.0005));
   efl_event_callback_add(_timer, EFL_LOOP_TIMER_EVENT_TICK, _timer_cb, NULL);
}
EFL_MAIN()

