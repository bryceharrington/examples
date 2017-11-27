#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Elementary.h>
#include <Efl_Ui.h>

/*
 * Efl.UI container exmaples.
 *
 * Load and pack a selection of containers.
 * Each has it's own unique layout and methods which are demonstrated below.
 */

/*
 * TODO Layout
 * TODO - still ELM Conformant
 * TODO - still ELM Mapbuf
 * TODO - still ELM Naviframe
 * TODO - still ELM Scroller
 * TODO - still ELM Table
 */

// Load a simple grid layout into the window
static Efl_Ui_Grid *
_ui_grid_setup(Efl_Ui_Win *win)
{
   Efl_Ui_Grid *grid;
   int i;

   grid = efl_add(EFL_UI_GRID_CLASS, win);

   efl_pack_grid_columns_set(grid, 2);
   efl_pack_grid_direction_set(grid, EFL_UI_DIR_RIGHT, EFL_UI_DIR_DOWN);

   efl_add(EFL_UI_BUTTON_CLASS, win,
           efl_text_set(efl_added, "Long Button"),
           efl_pack_grid(grid, efl_added, 0, 2, 2, 1));

   for (i = 1; i <= 4; i++)
     {
        efl_add(EFL_UI_BUTTON_CLASS, win,
                efl_text_set(efl_added, eina_slstr_printf("Grid %d", i)),
                efl_pack(grid, efl_added));
     }

   return grid;
}

// Load some boxes - a horizontal one for the window layout and a vertical
// one to contain a flow
static void
_ui_boxes_setup(Efl_Ui_Win *win)
{
   Efl_Ui_Box *hbox, *box, *button;
   int i;

   hbox = efl_add(EFL_UI_BOX_CLASS, win,
                 efl_ui_direction_set(efl_added, EFL_UI_DIR_HORIZONTAL),
                 efl_pack_padding_set(efl_added, 5, 0, EINA_TRUE),
                 efl_content_set(win, efl_added));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                 efl_pack(hbox, efl_added));
   efl_pack(hbox, _ui_grid_setup(win));

   for (i = 1; i <= 4; i++)
     {
        button = efl_add(EFL_UI_BUTTON_CLASS, win,
                         efl_text_set(efl_added, eina_slstr_printf("Boxed %d", i)),
                         efl_pack(box, efl_added));

        if (i == 2)
          efl_gfx_size_hint_max_set(button, EINA_SIZE2D(100, 50));
     }

}

// Load a vertical and horizontal split into the window
// TODO re-enable this when it actually works in EFL
static void
_ui_panes_setup(Efl_Ui_Win *win)
{
   Efl_Ui_Panes *split, *horiz_split;

   split = efl_add(EFL_UI_PANES_CLASS, win,
                   efl_content_set(win, efl_added),
                   efl_ui_panes_split_ratio_set(efl_added,  0.75));

   efl_add(EFL_UI_BOX_CLASS, win,
           efl_content_set(efl_part(split, "first"), efl_added));

   horiz_split = efl_add(EFL_UI_PANES_CLASS, win,
                         efl_content_set(efl_part(split, "second"), efl_added),
                         efl_ui_panes_split_ratio_set(efl_added,  0.85));

   efl_add(EFL_UI_BOX_CLASS, win,
           efl_content_set(efl_part(horiz_split, "first"), efl_added));

   efl_add(EFL_UI_BUTTON_CLASS, win,
           efl_text_set(efl_added, "Quit"),
           efl_content_set(efl_part(horiz_split, "second"), efl_added));
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *win;

   win = efl_add(EFL_UI_WIN_CLASS, NULL,
                 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
                 efl_text_set(efl_added, "Hello World"),
                 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

//   _ui_panes_setup(win);
   _ui_boxes_setup(win);

   efl_gfx_size_set(win, EINA_SIZE2D(350, 250));
}
EFL_MAIN()

