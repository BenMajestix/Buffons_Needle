#ifndef BUFFON_H_
#define BUFFON_H_

#include <SDL.h>
#include <gsl/gsl_rng.h>

#define FRAMERATE 30
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 500

#define needle_length 50
#define num_of_needles 8000

#define disp_width 100
#define disp_height 20


typedef struct {
  int x, y;
} point;

typedef struct {
  point p1, p2;
} line;

typedef struct {
  point p1, p2;
  int draw;
  // Degrees
  double angle;
} needle;

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  int running;
  int framerate;
} game;

void draw_lines(line lines[], int *num_of_lines);
void draw_needles (needle needles[]);

int min(int x, int y);
int max(int x, int y);

void initialize(void);
void terminate(int exit_code);
void handle_input(void);
double time_diff(struct timeval *start, struct timeval *end);
void incremental_draw_needles (int *amount_of_needles, int *num_of_intersections, line lines[], int num_of_lines);


#endif // BUFFON_H_
