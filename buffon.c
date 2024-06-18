#include "buffon.h"
#include "intersection.h"
#include "sdl_helpers.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>

gsl_rng* gsl_init_rand(void) {
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  struct timeval tv;
  gettimeofday(&tv, 0);
  unsigned long seed = tv.tv_sec + tv.tv_usec;
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  gsl_rng_set(r, seed);
  return r;
}

double gsl_rand_double(gsl_rng *r) {
    double u = gsl_rng_uniform(r);
    return u;
}

unsigned long int gsl_rand_int(gsl_rng *r, unsigned long int n) {
  unsigned long int u = gsl_rng_uniform_int(r, n);
  return u;
}

int main() {
  struct timeval randTime;
  gettimeofday(&randTime, NULL);
  srand((int) randTime.tv_usec);

  initialize();
  mainGame.framerate = FRAMERATE;
  struct timeval start, end;

  int width_betw_lines = needle_length*2;
  int num_of_lines = SCREEN_WIDTH / (width_betw_lines) - 1;
  line lines[num_of_lines];
  for (int i = 0; i < num_of_lines; i++) {
    lines[i].p1.x = width_betw_lines + (i * width_betw_lines);
    lines[i].p1.y = 0 - (needle_length*2);
    lines[i].p2.x = width_betw_lines + (i * width_betw_lines);
    lines[i].p2.y = SCREEN_HEIGHT + (needle_length*2);
  }
  draw_lines(lines, &num_of_lines);

  int amount_of_needles = 0;
  int num_of_intersections = 0;

  while (mainGame.running) {
    //mainGame.running = 0;

    gettimeofday(&start, NULL);
    //clear screen
    /*
    SDL_SetRenderDrawColor(mainGame.renderer, 0, 0, 0, 255);
    SDL_RenderClear(mainGame.renderer);
    */
    handle_input();
    //do stuff
    incremental_draw_needles(&amount_of_needles, &num_of_intersections, lines, num_of_lines);

    //calc intersections
    double res = (double) amount_of_needles / num_of_intersections;
    printf("Needles: %d | Intersections: %d\n", amount_of_needles, num_of_intersections);
    printf("Appr of Pi: %lf\n\n", res);

    //render
    SDL_RenderPresent(mainGame.renderer);

    gettimeofday(&end, NULL);

    double frametime = 1000 / mainGame.framerate;
    //SDL_Delay(frametime - (time_diff(&start, &end)));
  }
  terminate(0);

  return 0;
}

void draw_lines (line lines[], int *num_of_lines) {
  for(int i = 0; i < *num_of_lines; i++) {
    //printf("Line %d: x1:%d y1:%d x2:%d y2:%d\n", i, lines[i].p1.x, lines[i].p1.y, lines[i].p2.x, lines[i].p2.y);
    aalineRGBA(mainGame.renderer, lines[i].p1.x, lines[i].p1.y, lines[i].p2.x, lines[i].p2.y, 0, 255, 0, 255);
  }
}

void draw_needles (needle needles[]) {
  for (int i = 0; i < num_of_needles; i++) {
    if (needles[i].draw) {
      aalineRGBA(mainGame.renderer, needles[i].p1.x, needles[i].p1.y, needles[i].p2.x, needles[i].p2.y, 0, 0, 255, 255);
    }
    else {
      aalineRGBA(mainGame.renderer, needles[i].p1.x, needles[i].p1.y, needles[i].p2.x, needles[i].p2.y, 255, 0, 0, 255);
    }
  }
}

void incremental_draw_needles (int *amount_of_needles, int *num_of_intersections, line lines[], int num_of_lines) {
  gsl_rng *r = gsl_init_rand();
  //generate g new needles
  int g = 10;
  for (int i = 0; i < g; i++) {
    needle n;
    n.p1.x = gsl_rand_int(r, SCREEN_WIDTH - 2 * needle_length) + needle_length;
    n.p1.y = gsl_rand_int(r, SCREEN_HEIGHT);
    double angle = gsl_rand_double(r);

    int x = cos(angle * M_PI) * needle_length;
    int y = sin(angle * M_PI) * needle_length;
    //saving the point in the global coord system
    n.p2.x = n.p1.x + x;
    n.p2.y = n.p1.y + y;

    n.draw = 1;

    for (int j = 0; j < num_of_lines; j++) {
      if (do_intersect(n.p1, n.p2, lines[j].p1, lines[j].p2)) {
        aalineRGBA(mainGame.renderer, n.p1.x, n.p1.y, n.p2.x, n.p2.y, 255, 0, 0, 255);
        *num_of_intersections += 1;
        break;
      }
      else {
        aalineRGBA(mainGame.renderer, n.p1.x, n.p1.y, n.p2.x, n.p2.y, 0, 0, 255, 255);
      }
    }
    *amount_of_needles += 1;
  }
  gsl_rng_free(r);
}










int min(int x, int y) { return (x < y) ? x : y; }

int max(int x, int y) { return (x > y) ? x : y; }
