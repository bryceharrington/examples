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

void draw_tile(cairo_t *cr, int c, int r, int tile_width, int tile_depth) {
   cairo_set_line_width(cr, 0.5);
   cairo_set_source_rgb(cr, 0.0, 0.0, 0);

   cairo_move_to(cr, c*tile_width, r*tile_depth);
   cairo_line_to(cr, c*tile_width, (r+1)*tile_depth);
   cairo_line_to(cr, (c+1)*tile_width, (r+1)*tile_depth);
   cairo_line_to(cr, (c+1)*tile_width, r*tile_depth);
   cairo_close_path(cr);

   cairo_stroke_preserve(cr);

   cairo_set_source_rgb(cr, 0.0, 0.4, 0);
   cairo_fill(cr);
}

void draw_panel(cairo_t *cr, double color, int z0, int z1, int ax, int ay, int bx, int by) {
  cairo_set_line_width(cr, 0.5);
  cairo_set_source_rgb(cr, 0.0, 0.0, 0);
  cairo_move_to(cr, ax - z0, ay - z0);
  cairo_line_to(cr, ax - z1, ay - z1);
  cairo_line_to(cr, bx - z1, by - z1);
  cairo_line_to(cr, bx - z0, by - z0);
  cairo_close_path(cr);
  cairo_stroke_preserve(cr);
  cairo_set_source_rgb(cr, color, color, color);
  cairo_fill(cr);
}

void draw_platform(cairo_t *cr, double color, int height, int ax, int ay, int bx, int by) {
  cairo_set_line_width(cr, 0.5);
  cairo_set_source_rgb(cr, 0.0, 0.0, 0);
  cairo_move_to(cr, ax - height, ay - height);
  cairo_line_to(cr, ax - height, by - height);
  cairo_line_to(cr, bx - height, by - height);
  cairo_line_to(cr, bx - height, ay - height);
  cairo_close_path(cr);
  cairo_stroke_preserve(cr);
  cairo_set_source_rgb(cr, color, color, color);
  cairo_fill(cr);
}

void draw_wall(cairo_t *cr, int c, int r, int x_inset, int y_inset, int height, int tile_width, int tile_depth) {
  int x0 = c*tile_width;
  int y0 = r*tile_depth;
  int wall_width = tile_width - 2*x_inset;
  int wall_depth = tile_depth - 2*y_inset;

  // Front Right Side
  draw_panel(cr, 0.8, 0, height,
	    x0 + x_inset + wall_width, y0 + y_inset,
	    x0 + x_inset + wall_width, y0 + y_inset + wall_depth);

  // Front Left Side
  draw_panel(cr, 0.7, 0, height,
	    x0 + x_inset,               y0 + y_inset + wall_depth,
	    x0 + x_inset + wall_width,  y0 + y_inset + wall_depth);

  // Top
  draw_platform(cr, 0.6, height,
		x0 + x_inset,              y0 + y_inset,
		x0 + x_inset + wall_width, y0 + y_inset + wall_depth);
}


static void
_gui_setup()
{
   Eo *win, *box;
   Evas_Object *source_image;  // TODO: Should Evas_Object* really be an Eo*?
   unsigned char *cairo_pixels;
   unsigned char *efl_pixels;
   cairo_matrix_t matrix;
   int tile_width = 40;
   int tile_depth = 40;
   int wall_height = 24;
   int wall_inset = 12;

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

   cairo_matrix_init(&matrix,
		     1, 0.5,
		     -1, 0.5,
		     0, -1);
   cairo_translate(cr, 400, 50);
   cairo_transform(cr, &matrix);
   for (int c=0; c<10; c++) {
     for (int r=0; r<10; r++) {
       draw_tile(cr, c, r, tile_width, tile_depth);
     }
   }
   draw_wall(cr, 0, 0,          0,          0, wall_height, tile_width, tile_depth); // corner
   draw_wall(cr, 0, 1, wall_inset,          0, wall_height, tile_width, tile_depth);
   draw_wall(cr, 0, 2, wall_inset,          0, wall_height, tile_width, tile_depth);
   draw_wall(cr, 0, 3,          0,          0, wall_height, tile_width, tile_depth); // corner
   draw_wall(cr, 1, 0,          0, wall_inset, wall_height, tile_width, tile_depth);
   draw_wall(cr, 1, 3,          0, wall_inset, wall_height, tile_width, tile_depth);
   draw_wall(cr, 2, 0,          0,          0, wall_height, tile_width, tile_depth); // corner
   draw_wall(cr, 2, 1, wall_inset,          0, wall_height, tile_width, tile_depth);
   draw_wall(cr, 2, 3,          0,          0, wall_height, tile_width, tile_depth); // corner

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
