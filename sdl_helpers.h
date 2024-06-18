#include <SDL.h>

game mainGame = {
  .running = 1
};

void initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error: failed to initialize SDL: %s\n", SDL_GetError());
    terminate(EXIT_FAILURE);
  }

  // create the mainGame window
  mainGame.window = SDL_CreateWindow("Buffons Needle", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!mainGame.window) {
    printf("error: failed to open %d x %d window: %s\n", SCREEN_WIDTH,
           SCREEN_HEIGHT, SDL_GetError());
    terminate(EXIT_FAILURE);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  mainGame.renderer = SDL_CreateRenderer(mainGame.window, -1, SDL_RENDERER_ACCELERATED);

  if (!mainGame.renderer) {
    printf("error: failed to create renderer: %s\n", SDL_GetError());
    terminate(EXIT_FAILURE);
  }
}

void terminate(int exit_code) {
  if (mainGame.renderer) {
    SDL_DestroyRenderer(mainGame.renderer);
  }
  if (mainGame.window) {
    SDL_DestroyWindow(mainGame.window);
  }
  SDL_Quit();
  exit(exit_code);
}

void handle_input() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    // change the mainGame state to not running when close or the esc key is pressed
    // so that the mainGame loop is exited in main
    if (e.type == SDL_QUIT ||
        (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
      mainGame.running = 0;
    }
  }
}

double time_diff(struct timeval *start, struct timeval *end) {
  return (end->tv_sec - start->tv_sec) + 1e-3 * (end->tv_usec - start->tv_usec);
}
