#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

static void
_gui_about_clicked_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *button = data;

   printf("Clicked About\n");
}

static void
_gui_quit_clicked_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   efl_exit(0);
}

static void
_gui_setup()
{
   Eo *win, *box, *hbox, *editor, *button;

   // TODO can we have nicer methods like efl_ui_win_add?
   win = efl_add(EFL_UI_WIN_CLASS, NULL,
                 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
                 efl_text_set(efl_added, "Hello World"),
                 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                efl_content_set(win, efl_added),
                efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(360, 240)));

   editor = efl_add(EFL_UI_TEXT_CLASS, box,
                    efl_text_set(efl_added, "Edit me"),
                    efl_ui_text_interactive_editable_set(efl_added, EINA_TRUE),
                    efl_ui_text_scrollable_set(efl_added, EINA_TRUE),
                    efl_pack(box, efl_added));

   hbox = efl_add(EFL_UI_BOX_CLASS, box,
                 efl_ui_direction_set(efl_added, EFL_UI_DIR_HORIZONTAL),
                 efl_gfx_size_hint_weight_set(efl_added, 1.0, 0.1),
                 efl_pack(box, efl_added));
   button = efl_add(EFL_UI_BUTTON_CLASS, hbox,
                    efl_text_set(efl_added, "About"),
                    efl_pack(hbox, efl_added),
                    efl_event_callback_add(efl_added, EFL_UI_EVENT_CLICKED,
                                           _gui_about_clicked_cb, efl_added));
   button = efl_add(EFL_UI_BUTTON_CLASS, hbox,
                    efl_text_set(efl_added, "Quit"),
                    efl_pack(hbox, efl_added),
                    efl_event_callback_add(efl_added, EFL_UI_EVENT_CLICKED,
                                           _gui_quit_clicked_cb, efl_added));
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   // TODO this to a efl_ui_policy method
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _gui_setup();
}
EFL_MAIN()

