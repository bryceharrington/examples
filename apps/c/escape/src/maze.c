// https://rosettacode.org/wiki/Maze_generation#C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define DOUBLE_SPACE 0

#if DOUBLE_SPACE
#define SPC "  "
#else
#define SPC " "
#endif

wchar_t glyph[] = L""SPC"│││─┘┐┤─└┌├─┴┬┼"SPC"┆┆┆┄╯╮ ┄╰╭ ┄";

typedef unsigned char byte;
enum { N = 1, S = 2, W = 4, E = 8, V = 16 };

int w, h, avail;
#define each(i, x, y) for (i = x; i <= y; i++)

typedef struct _maze {
  byte **cell;
} maze_t;

int irand(int n)
{
  int r, rmax = n * (RAND_MAX / n);
  while ((r = rand()) >= rmax);
  return r / (RAND_MAX/n);
}

void print_maze_unicode(maze_t *maze)
{
  int i, j, c;
  each(i, 0, 2 * h) {
    each(j, 0, 2 * w) {
      c = maze->cell[i][j];
      if (c > V) printf("\033[31m");
      printf("%lc", glyph[c]);
      if (c > V) printf("\033[m");
      }
    putchar('\n');
    }
}

inline int max(int a, int b) { return a >= b ? a : b; }
inline int min(int a, int b) { return b >= a ? a : b; }

static int dirs[4][2] = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};
void walk_maze(maze_t *maze, int x, int y)
{
  int i, t, x1, y1, d[4] = { 0, 1, 2, 3 };

  maze->cell[y][x] |= V;
  avail--;

  for (x1 = 3; x1; x1--)
    if (x1 != (y1 = irand(x1 + 1)))
      i = d[x1], d[x1] = d[y1], d[y1] = i;

  for (i = 0; avail && i < 4; i++) {
    x1 = x + dirs[ d[i] ][0], y1 = y + dirs[ d[i] ][1];

    if (maze->cell[y1][x1] & V) continue;

    /* break walls */
    if (x1 == x) {
      t = (y + y1) / 2;
      maze->cell[t][x+1] &= ~W, maze->cell[t][x] &= ~(E|W), maze->cell[t][x-1] &= ~E;
      } else if (y1 == y) {
      t = (x + x1)/2;
      maze->cell[y-1][t] &= ~S, maze->cell[y][t] &= ~(N|S), maze->cell[y+1][t] &= ~N;
      }
    walk_maze(maze, x1, y1);
    }
}

int solve_maze(maze_t *maze, int x, int y, int tox, int toy)
{
  int i, t, x1, y1;

  maze->cell[y][x] |= V;
  if (x == tox && y == toy) return 1;

  each(i, 0, 3) {
    x1 = x + dirs[i][0], y1 = y + dirs[i][1];
    if (maze->cell[y1][x1]) continue;

    /* mark path */
    if (x1 == x) {
      t = (y + y1)/2;
      if (maze->cell[t][x] || !solve_maze(maze, x1, y1, tox, toy)) continue;

      maze->cell[t-1][x] |= S, maze->cell[t][x] |= V|N|S, maze->cell[t+1][x] |= N;
      } else if (y1 == y) {
      t = (x + x1)/2;
      if (maze->cell[y][t] || !solve_maze(maze, x1, y1, tox, toy)) continue;

      maze->cell[y][t-1] |= E, maze->cell[y][t] |= V|E|W, maze->cell[y][t+1] |= W;
      }
    return 1;
    }

  /* backtrack */
  maze->cell[y][x] &= ~V;
  return 0;
}

void create_maze(maze_t *maze, int width, int height)
{
  int i, j, h2, w2;
  byte **p;

  w = width;  // TODO
  h = height;

  h2 = 2 * h + 2;
  w2 = 2 * w + 2;

  p = calloc(sizeof(byte*) * (h2 + 2) + w2 * h2 + 1, 1);

  p[1] = (byte*)(p + h2 + 2) + 1;
  each(i, 2, h2) p[i] = p[i-1] + w2;
  p[0] = p[h2];
  maze->cell = &p[1];

  each(i, -1, 2 * h + 1) maze->cell[i][-1] = maze->cell[i][w2 - 1] = V;
  each(j, 0, 2 * w) maze->cell[-1][j] = maze->cell[h2 - 1][j] = V;
  each(i, 0, h) each(j, 0, 2 * w) maze->cell[2*i][j] |= E|W;
  each(i, 0, 2 * h) each(j, 0, w) maze->cell[i][2*j] |= N|S;
  each(j, 0, 2 * w) maze->cell[0][j] &= ~N, maze->cell[2*h][j] &= ~S;
  each(i, 0, 2 * h) maze->cell[i][0] &= ~W, maze->cell[i][2*w] &= ~E;

  avail = w * h;
  walk_maze(maze, irand(2) * 2 + 1, irand(h) * 2 + 1);

  /* reset visited marker (it's also used by path finder) */
  each(i, 0, 2 * h) each(j, 0, 2 * w) maze->cell[i][j] &= ~V;
  solve_maze(maze, 1, 1, 2 * w - 1, 2 * h - 1);

  print_maze_unicode(maze);
}
