#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

static void 
_gui_setup()
{
   Evas_Object *win, *box;

   // TODO can we have nicer methods like efl_ui_win_add?
   win = efl_add(EFL_UI_WIN_CLASS, NULL,
                 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
                 efl_text_set(efl_added, "Hello World"),
                 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                efl_content_set(win, efl_added),
                efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(360, 240)));

   efl_add(EFL_UI_TEXT_CLASS, box,
           efl_text_set(efl_added, "Hello World!"),
           efl_gfx_size_hint_align_set(efl_added, 0.5, 0.5),
           efl_pack(box, efl_added));
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   // TODO this to a efl_ui_policy method
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _gui_setup();
}
EFL_MAIN()

