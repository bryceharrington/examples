#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>
#include <Evas.h>

static void
_gui_editor_changed_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *editor = data;

   printf("Change recorded: %s\n", efl_text_get(editor));
}

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

   win = efl_add(EFL_UI_WIN_CLASS, NULL,
                 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
                 efl_text_set(efl_added, "Escape"),
                 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                efl_content_set(win, efl_added),
                efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(360, 240)));

   // TODO: efl_ui_bg_class_get() to create a colorable background

   // TODO: Turn this into a canvas
   //  * Look at the efl_ API docs for any related to canvas
   //  * I need something like evas_object_vg_add()
   //  * ./lib/evas/canvas/efl_vg_gradient_linear.eo.h:typedef
   editor = efl_add(EFL_UI_TEXT_CLASS, box,
                    efl_text_set(efl_added, "Edit me"),
                    efl_ui_text_interactive_editable_set(efl_added, EINA_TRUE),
                    efl_ui_text_scrollable_set(efl_added, EINA_TRUE),
                    efl_pack(box, efl_added));
   efl_event_callback_add(editor, EFL_UI_TEXT_EVENT_CHANGED_USER,
                          _gui_editor_changed_cb, editor);

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
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _gui_setup();
}
EFL_MAIN()

