#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Efl Core Poll examples.
 *
 * This example sets up poll callbacks for LOW, MEDIUM and HIGH frequency events.
 * We run for 30 seconds and print to stdout to show when each is called.
 * Depending on your system this may not include any LOW frequency polls.
 */

// the poll callback for low frequency events
static void
_poll_low_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("L");
}

// the poll callback for medium frequency events
static void
_poll_med_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("M");
}

// the poll callback for high frequency events
static void
_poll_high_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf(".");

}

// Our timer callback ticks at the specified interval to interrupt the polling
static void
_timer_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop_Timer *timer;

   timer = event->object;

   printf("\nTIMER: timer callback called, exiting.\n");

   efl_del(timer);
   efl_exit(0);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Efl_Loop *loop = ev->object;

   // don't buffer output this time as we're looking for immediate output
   setbuf(stdout, NULL);

   efl_event_callback_add(loop, EFL_LOOP_EVENT_POLL_LOW, _poll_low_cb, NULL);
   efl_event_callback_add(loop, EFL_LOOP_EVENT_POLL_MEDIUM, _poll_med_cb, NULL);
   efl_event_callback_add(loop, EFL_LOOP_EVENT_POLL_HIGH, _poll_high_cb, NULL);

   efl_add(EFL_LOOP_TIMER_CLASS, loop,
           efl_loop_timer_interval_set(efl_added, 30),
           efl_event_callback_add(efl_added, EFL_LOOP_TIMER_EVENT_TICK, _timer_cb, NULL));
}
EFL_MAIN()

