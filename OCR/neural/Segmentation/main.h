# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>

# include "pixel_operations.h"
# include <SDL.h>
# include <SDL_image.h>

void wait_for_keypressed(void);

void init_sdl(void);

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

int pixel_test(size_t x, size_t y);

size_t defineSliceHeight();

void defineMargin(size_t* leftMargin, size_t* rightMargin, size_t* upMargin, size_t* lowMargin, size_t left, size_t right, size_t up, size_t down);

void draw_line(size_t x1, size_t y1, size_t x2, size_t y2);

int slice_test(size_t x, size_t y);

void cut_chara(struct queue *queue, size_t leftMargin, size_t upMargin, size_t lowMargin, size_t len);

void cut_line(struct queue *queue, size_t leftMargin, size_t rightMargin, size_t upMargin, size_t lowMargin);

void cut_block(struct queue *queue, size_t left, size_t right, size_t up, size_t low);

//int main (int argc, char* argv[]);
