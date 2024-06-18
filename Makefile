CC = gcc
CFLAGS = -g
DEPS = buffon.h intersection.h sdl_helpers.h
OBJ = buffon.o
SDLLIBS = -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib -pthread -lSDL2 -lSDL2_gfx
GSLLIBS = -lgsl -lgslcblas -lm
LIBS = $(SDLLIBS) $(GSLLIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

buffon: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
