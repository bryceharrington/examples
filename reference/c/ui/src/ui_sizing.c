#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Elementary.h>
#include <Efl_Ui.h>

/*
 * Efl.UI sizing exmaples.
 *
 * Demonstrate how to use the sizing api from Efl.Gfx.
 * We load a box with 3 buttons, one with default sizing, one that has a max
 * and the last has a min size. Try resizing the window to see how this changes.
 */

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Efl_Ui_Win *win, *box;

   win = efl_add(EFL_UI_WIN_CLASS, NULL,
                 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
                 efl_text_set(efl_added, "Size Control"),
                 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                 efl_content_set(win, efl_added));

   efl_add(EFL_UI_BUTTON_CLASS, win,
           efl_text_set(efl_added, "Button"),
           efl_pack_end(box, efl_added));

   efl_add(EFL_UI_BUTTON_CLASS, win,
           efl_text_set(efl_added, "Small"),
           efl_pack_end(box, efl_added),
           efl_gfx_size_hint_max_set(efl_added, EINA_SIZE2D(50, 50)));

   efl_add(EFL_UI_BUTTON_CLASS, win,
           efl_text_set(efl_added, "Big Button"),
           efl_pack_end(box, efl_added),
           efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(100, 100)));

   efl_gfx_size_set(win, EINA_SIZE2D(320, 320));
}
EFL_MAIN()

