#include <math.h>
#include <cairo.h>

#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>
#include <Evas.h>

#define WIDTH 800
#define HEIGHT 640

static cairo_surface_t *surface;
static cairo_t *cr;

static void
_gui_setup()
{
   Eo *win, *box;
   Evas_Object *source_image;  // TODO: Should Evas_Object* really be an Eo*?
   unsigned char *cairo_pixels;
   unsigned char *efl_pixels;

   win = efl_add(EFL_UI_WIN_CLASS, NULL,
		 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
		 efl_text_set(efl_added, "Escape"),
		 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   // Set default window size to our desired geometry
   box = efl_add(EFL_UI_BOX_CLASS, win,
		efl_content_set(win, efl_added),
		efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(WIDTH, HEIGHT)));

   source_image = efl_add(EFL_CANVAS_IMAGE_CLASS, box);
   efl_gfx_buffer_copy_set(source_image, NULL, EINA_SIZE2D(WIDTH, HEIGHT), 0, EFL_GFX_COLORSPACE_ARGB8888, 0);
   efl_gfx_position_set(source_image, EINA_POSITION2D(0, 0));
   efl_gfx_size_set(source_image, EINA_SIZE2D((WIDTH),  (HEIGHT)));
   efl_gfx_visible_set(source_image, EINA_TRUE);

   surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
   cr = cairo_create(surface);
   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
   cairo_arc(cr, IMAGE_WIDTH / 2, IMAGE_HEIGHT / 2, IMAGE_WIDTH / 4.0, 0.0, 2.0 * M_PI);
   cairo_fill(cr);

   cairo_pixels = cairo_image_surface_get_data(surface);

   int stride;
   Eina_Rw_Slice slice;
   slice = efl_gfx_buffer_map(source_image, EFL_GFX_BUFFER_ACCESS_MODE_WRITE, NULL,
			      EFL_GFX_COLORSPACE_ARGB8888, 0, &stride);
   efl_pixels = slice.mem;
   if (!efl_pixels) return;

   // Get pixels into source_image
   // TODO: Isn't there a more direct way to do this?
   int i, j;
   for (i = 0; i<HEIGHT; i++) {
      unsigned int *source_row = (unsigned int *)((char *)cairo_pixels + stride * i);
      unsigned int *dest_row = (unsigned int *)((char *)efl_pixels + stride * i);

      for (j = 0; j < WIDTH; j++) {
	 // Copy cairo_pixels to efl_pixels
	 *dest_row++ = (*source_row++) | 0xff000000;
      }
   }

   efl_gfx_buffer_unmap(source_image, slice);
   efl_gfx_buffer_update_add(source_image, NULL);
}


EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _gui_setup();
}
EFL_MAIN()
