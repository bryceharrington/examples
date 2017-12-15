#include <stdlib.h> // For srandom
#include <locale.h> // For setlocale
#include <time.h>   // For time
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

// Game logic processes at a tick rate of 60 Hz
// We'll update graphics at the same rate, for 60 FPS
#define GAME_TICK_INTERVAL (1.0/60.0)

typedef struct _game {
  int grid_width;
  int grid_height;
  int player_c;
  int player_r;
  int goal_c;
  int goal_r;
} game_t;

// TODO: Move to a header
typedef unsigned char byte;
typedef struct _maze {
  byte **cell;
} maze_t;
extern void create_maze(maze_t *maze, int width, int height);
extern void print_maze_unicode(maze_t *maze);

static cairo_t *cr;
static game_t game;

static Evas_Object *source_image;  // TODO: Should Evas_Object* really be an Eo*?

static void
draw_tile(cairo_t *cr, int c, int r, int tile_width, int tile_depth) {
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

static void
draw_panel(cairo_t *cr, double color, int z0, int z1, int ax, int ay, int bx, int by) {
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

static void
draw_platform(cairo_t *cr, double color, int height, int ax, int ay, int bx, int by) {
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

static void
draw_wall(cairo_t *cr, int c, int r, int x_inset, int y_inset, int height, int tile_width, int tile_depth) {
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
draw_ball(cairo_t *cr, int c, int r, int tile_width, int tile_depth) {
  double xc = c*tile_width + tile_width/2.0;
  double yc = r*tile_depth + tile_depth/2.0;

  // Shadow
  cairo_arc(cr, xc, yc, tile_width/4.0, 0, 2*M_PI);
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.25);
  cairo_fill(cr);

  // Ball
  cairo_save(cr);
  xc -= tile_width/3.0;
  yc -= tile_depth/3.0;
  cairo_user_to_device(cr, &xc, &yc);
  cairo_identity_matrix(cr);
  cairo_arc(cr, xc, yc, tile_width/3.0, 0, 2*M_PI);
  cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
  cairo_fill(cr);
  cairo_restore(cr);
}

static void
draw_diamond(cairo_t *cr, int c, int r, int tile_width, int tile_depth) {
  double xc = c*tile_width + tile_width/2.0;
  double yc = r*tile_depth + tile_depth/2.0;

  // Shadow
  cairo_arc(cr, xc, yc, tile_width/4.0, 0, 2*M_PI);
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.10);
  cairo_fill(cr);

  // Diamond
  cairo_save(cr);
  xc -= tile_width/8.0;
  yc -= tile_depth/8.0;
  cairo_user_to_device(cr, &xc, &yc);
  cairo_identity_matrix(cr);

  // Diamond Body
  cairo_move_to(cr, xc, yc);
  cairo_rel_line_to(cr, -15, -15);
  cairo_rel_line_to(cr,   5, -10);
  cairo_rel_line_to(cr,  20,   0);
  cairo_rel_line_to(cr,   5,  10);
  cairo_close_path(cr);
  cairo_set_source_rgba(cr, 0.5, 0.5, 0.9, 0.75);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.75);
  cairo_stroke(cr);

  // Diamond Top Face
  cairo_move_to(cr, xc-7, yc-13);
  cairo_rel_line_to(cr,   5, -12);
  cairo_rel_line_to(cr,   4,   0);
  cairo_rel_line_to(cr,   4,  12);
  cairo_close_path(cr);
  cairo_set_source_rgba(cr, 0.5, 0.5, 0.9, 0.5);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_stroke(cr);

  // Diamond Bottom Face
  cairo_move_to(cr, xc, yc);
  cairo_rel_line_to(cr,  -7, -13);
  cairo_rel_line_to(cr,  12,   0);
  cairo_close_path(cr);
  cairo_set_source_rgba(cr, 0.5, 0.5, 0.9, 0.5);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_stroke(cr);

  // Remaining face 1
  cairo_move_to(cr, xc-15, yc-15);
  cairo_rel_line_to(cr,   8,   2);
  cairo_rel_line_to(cr,  -2,  -12);
  cairo_close_path(cr);
  cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 0.75);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.75);
  cairo_stroke(cr);

  // Remaining face 2
  cairo_move_to(cr, xc+15, yc-15);
  cairo_rel_line_to(cr,  -8,   2);
  cairo_rel_line_to(cr,   2,  -12);
  cairo_close_path(cr);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_stroke(cr);

  cairo_restore(cr);
}

static void
_key_down_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
  char *keys = efl_input_key_compose_get(event->info);
  if (keys == NULL)
    return;
  switch (keys[0]) {
    case 'w':
    case 'W':
      printf("UP\n");
      game.player_c -= 1;
      break;
    case 'a':
    case 'A':
      printf("LEFT\n");
      game.player_r += 1;
      break;
    case 'd':
    case 'D':
      printf("RIGHT\n");
      game.player_r -= 1;
      break;
    case 's':
    case 'S':
      printf("DOWN\n");
      game.player_c += 1;
      break;
    default:
      printf("key=%s keyname=%s string=%s compose=%s",
	     efl_input_key_get(event->info),
	     efl_input_key_name_get(event->info),
	     efl_input_key_string_get(event->info),
	     efl_input_key_compose_get(event->info));
      break;
    }
}

static void
_key_up_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   printf("\n");
}

EFL_CALLBACKS_ARRAY_DEFINE(key_callbacks,
			   { EFL_EVENT_KEY_DOWN, _key_down_cb },
			   { EFL_EVENT_KEY_UP, _key_up_cb })

static void
_draw_screen() {
   cairo_matrix_t matrix;
   int tile_width = 40;
   int tile_depth = 40;
   int wall_height = 24;
   int wall_inset = 12;

   // TODO: Technically only need to set up transform when surface is created
   cairo_identity_matrix(cr);
   cairo_matrix_init(&matrix,
		     1, 0.5,
		     -1, 0.5,
		     0, -1);
   cairo_translate(cr, 400, 50);
   cairo_transform(cr, &matrix);

   // Clear screen
   cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
   cairo_paint(cr);

   // Draw the grid
   for (int c=0; c<game.grid_width; c++) {
     for (int r=0; r<game.grid_height; r++) {
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

   draw_ball(cr, game.player_c, game.player_r, tile_width, tile_depth);
   draw_diamond(cr, game.goal_c, game.goal_r, tile_width, tile_depth);

   efl_gfx_buffer_update_add(source_image, NULL);
}

static void
_gui_setup()
{
   Eo *win, *box;
   int stride;
   Eina_Rw_Slice slice;
   unsigned char *efl_pixels;
   static cairo_surface_t *surface;

   win = efl_add(EFL_UI_WIN_CLASS, NULL,
		 efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
		 efl_text_set(efl_added, "Escape"),
		 efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   efl_event_callback_array_add(win, key_callbacks(), NULL);

   // Set default window size to our desired geometry
   box = efl_add(EFL_UI_BOX_CLASS, win,
		efl_content_set(win, efl_added),
		efl_gfx_size_hint_min_set(efl_added, EINA_SIZE2D(WIDTH, HEIGHT)));

   source_image = efl_add(EFL_CANVAS_IMAGE_CLASS, box);
   efl_gfx_buffer_copy_set(source_image, NULL, EINA_SIZE2D(WIDTH, HEIGHT), 0, EFL_GFX_COLORSPACE_ARGB8888, 0);
   efl_gfx_position_set(source_image, EINA_POSITION2D(0, 0));
   efl_gfx_size_set(source_image, EINA_SIZE2D((WIDTH),  (HEIGHT)));
   efl_gfx_visible_set(source_image, EINA_TRUE);

   slice = efl_gfx_buffer_map(source_image, EFL_GFX_BUFFER_ACCESS_MODE_WRITE, NULL,
			      EFL_GFX_COLORSPACE_ARGB8888, 0, &stride);
   efl_pixels = slice.mem;
   if (!efl_pixels) return;

   surface = cairo_image_surface_create_for_data(efl_pixels,
						 CAIRO_FORMAT_ARGB32,
						 WIDTH,
						 HEIGHT,
						 stride);
   cr = cairo_create(surface);

   efl_gfx_buffer_unmap(source_image, slice);
}

static void
_game_tick_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   // Efl_Loop_Timer *timer;
   // timer = event->object;

   //printf("\n[TICK]\n");
   _draw_screen();
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop *loop = event->object;
   int maze_width = 5;
   int maze_height = 5;
   maze_t maze;

   create_maze(&maze, maze_width, maze_height);

   // Grid size is 2*W+1, 2*H+1
   game.grid_width = 2 * maze_width + 1;
   game.grid_height = 2 * maze_height + 1;

   // TODO: Derive from the maze's solution
   game.player_c = 1;
   game.player_r = 8;
   game.goal_c = 6;
   game.goal_r = 3;

   setlocale(LC_ALL, "");
   srandom(time(NULL));

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   efl_add(EFL_LOOP_TIMER_CLASS, loop,
	   efl_loop_timer_interval_set(efl_added, GAME_TICK_INTERVAL),
	   efl_event_callback_add(efl_added, EFL_LOOP_TIMER_EVENT_TICK, _game_tick_cb, NULL));

   _gui_setup();
}
EFL_MAIN()
