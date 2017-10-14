# define _XOPEN_SOURCE 500

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>

# include "queue.h"
# include "main.h"
# include "pixel_operations.h"
# include <SDL.h>
# include <SDL_image.h>

size_t sliceHeight;
SDL_Surface* img;

void wait_for_keypressed(void) {
	SDL_Event event;
	for (;;) {
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN: return;
			default: break;
		}
	}
}

void init_sdl(void) {
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
	}
}

SDL_Surface* load_image(char *path) {

	SDL_Surface *img;
	img = IMG_Load(path);

	if (!img) {
		errx(3, "can't load %s: %s", path, IMG_GetError());
	}

	return img;
}

SDL_Surface* display_image(SDL_Surface *img) {

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);

	if (screen == NULL) {
		errx(1, "Couldn't set %dx%d video mode: %s\n",
			img->w, img->h, SDL_GetError());
	}

	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0) {
		warnx("BlitSurface error: %s\n", SDL_GetError());
	}

	SDL_UpdateRect(screen, 0, 0, img->w, img->h);
	wait_for_keypressed();

	return screen;
}

int pixel_test(size_t x, size_t y) {

	Uint8 r,g,b;
	Uint32 pixel = getpixel(img, x, y);
	SDL_GetRGB(pixel, img->format, &r, &g, &b);

	return (r < 110 && g < 110 && b < 110);
}

size_t defineSliceHeight() {

	int i = 0;
	size_t j1 = 0;

	for (int j = 0; j < img->h && j1 == 0; j++) {

		i = 0;

		while (i < img->w && !pixel_test(i, j)) {
			i++;
		}

		if (i < img->w) {
			j1 = j;
		}
	}

	size_t j2 = 0;

	for (int j = j1; j < img->h && j2 == 0; j++) {

		i = 0;

                while (i < img->w && !pixel_test(i, j)) {
                        i++;
                }

                if (i == img->w) {
                        j2 = j;
                }
        }

	return (j2 - j1);
}

void defineMargin(size_t* leftMargin, size_t* rightMargin, size_t* upMargin, size_t* lowMargin, size_t left, size_t right, size_t up, size_t down) {

	for (size_t j = up; j < down && *upMargin == 0; j++) {
		for (size_t i = left; i < right && *upMargin == 0; i++) {

			if (pixel_test(i, j)) {
				*upMargin = j-1;
			}
		}
	}

	for (size_t j = down; j > up && *lowMargin == 0; j--) {
                for (size_t i = left; i < right && *lowMargin == 0; i++) {

                        if (pixel_test(i, j)) {
                        	*lowMargin = j+1;
                        }
                }
        }

	for (size_t i = left; i < right && *leftMargin == 0; i++) {
		for (size_t j = up; j < down && *leftMargin == 0; j++) {

			if (pixel_test(i, j)) {
                        	*leftMargin = i-1;
                        }
                }
        }

	for (size_t i = right; i > up && *rightMargin == 0; i--) {
                for (size_t j = up; j < down && *rightMargin == 0; j++) {

                        if (pixel_test(i, j)) {
                                *rightMargin = i+1;
                        }
                }
        }
}

void draw_line(size_t x1, size_t y1, size_t x2, size_t y2) {

	for (size_t i = x1; i <= x2; i++) {
		for (size_t j = y1; j <= y2; j++) {

			putpixel(img, i, j, SDL_MapRGB(img->format, 0, 0, 0));
		}
	}
}

int slice_test(size_t x, size_t y) {

	int res = 0;

	for (size_t j = y; j < y + sliceHeight && !res; j++) {
		res = pixel_test(x, j);
	}

	return (res);
}

double* saveLetter(size_t leftMargin, size_t rightMargin, size_t upMargin, size_t lowMargin) {

	double* data = malloc(((42 * 42) + 1) * sizeof (double));
	data[0] = 1;

	for (size_t i = 0; i < 42; i++) {
		for (size_t j = 0; j < 42; j++) {
			if (i == 0 && j == 0) {
				j++;
			}
			if (leftMargin + 1 + j < rightMargin && upMargin + i < lowMargin) {
				data[42*i + j] = !pixel_test(leftMargin + 1 + j, upMargin + i);
			} else {
				data[42*i + j] = 1;
			}
		}
	}

	return (data);
}

void cut_chara(struct queue *queue, size_t leftMargin, size_t upMargin, size_t lowMargin, size_t rightMargin) {

	int isText;
	int wasText = 0;
	if (queue) {}
	size_t charStart = leftMargin;

	for (size_t i = leftMargin; i < rightMargin + 1; i++) {

		isText = 0;

		for (size_t j = upMargin+1; j < lowMargin && !isText; j++) {

			isText = pixel_test(i,j);
		}

		if (!isText) {
			if (wasText) {
				draw_line(i, upMargin+1, i, lowMargin-1);
				wasText = 0;
				double *data = saveLetter(charStart, i, upMargin+1, lowMargin-1);
				queue_push(queue, data);
			}
		} else if (!wasText) {
			draw_line(i-1, upMargin+1, i-1, lowMargin-1);
			charStart = i-1;
		}

		wasText = isText;
	}
}

void cut_line(struct queue *queue, size_t leftMargin, size_t rightMargin, size_t upMargin, size_t lowMargin) {

	size_t endLine = leftMargin;
	size_t lineHeight = (120 * sliceHeight) / 100;
	printf("lineHeight = %lu\n", lineHeight);

	for (size_t j = 0; j <= (lowMargin - upMargin) / lineHeight; j++) {
		for (size_t i = leftMargin; i < rightMargin; i++) {

			if (slice_test(i, j * lineHeight + upMargin + 1)) {

				endLine = i + 1;
			}
		}

		draw_line(leftMargin, j * lineHeight + upMargin, endLine, j * lineHeight + upMargin);
		draw_line(leftMargin, (j + 1) * lineHeight + upMargin, endLine, (j + 1) * lineHeight + upMargin);
		cut_chara(queue, leftMargin, j * lineHeight + upMargin, (j+1) * lineHeight + upMargin, endLine);
		queue_push(queue, NULL);
	}
}

void cut_block(struct queue *queue, size_t left, size_t right, size_t up, size_t low) {

	size_t leftMargin = 0;
	size_t rightMargin = 0;
	size_t upMargin = 0;
	size_t lowMargin = 0;

	defineMargin(&leftMargin, &rightMargin, &upMargin, &lowMargin, left+1, right-1, up+1, low-1);

	printf("leftMargin = %lu\n", leftMargin);
        printf("rightMargin = %lu\n", rightMargin);
        printf("upMargin = %lu\n", upMargin);
        printf("lowMargin = %lu\n\n", lowMargin);

	int canCutHor = 1;

	for (size_t i = upMargin; i < lowMargin && canCutHor; i++) {

		size_t j = leftMargin;

		while (j < rightMargin && canCutHor) {

			//putpixel(img, j, i, SDL_MapRGB(img->format, 255, 0, 0));
			if (slice_test(j, i)) {
			//	printf("[%lu,%lu]\n", i, j);
				canCutHor = 0;
			}

			j++;
		}

		if (canCutHor) {

			draw_line(leftMargin, i, rightMargin, i);

			cut_block(queue, leftMargin, rightMargin, upMargin, i-1);
			cut_block(queue, leftMargin, rightMargin, i+1, lowMargin);

			canCutHor = 0;
		} else {
			canCutHor = 1;
		}
	}

	int canCutVer = 1;

	for (size_t i = leftMargin; i < rightMargin && canCutVer; i++) {
                for (size_t j = upMargin; j < lowMargin && canCutVer; j++) {
			for (size_t k = 0; k < (40 * sliceHeight) / 100; k++) {
                        	if (slice_test(i+k, j)) {
                                	canCutVer = 0;
                        	}
			}
                }

                if (canCutVer) {

                        draw_line(i, upMargin, i, lowMargin);

                        cut_block(queue, i+1, rightMargin, upMargin, lowMargin);
                        cut_block(queue, leftMargin, i-1, upMargin, lowMargin);

			canCutVer = 0;
                } else {
			canCutVer = 1;
		}
        }

	if (canCutHor && canCutVer) {
		cut_line(queue, leftMargin, rightMargin, upMargin, lowMargin);
	}
}
/*
int main (int argc, char* argv[]) {

        if (argc == 2) {
                init_sdl();
                img = load_image(argv[1]);
		display_image(img);

		sliceHeight = defineSliceHeight();
		printf("sliceHeight = %lu\n", sliceHeight);

		size_t leftMargin = 0;
                size_t rightMargin = 0;
                size_t upMargin = 0;
                size_t lowMargin = 0;

		defineMargin(&leftMargin, &rightMargin, &upMargin, &lowMargin, 0, img->w - 1, 0, img->h - 1);

		struct queue* queue;
		queue = malloc(sizeof (struct queue));
		queue->store = NULL;
		queue->size = 0;

		cut_block(queue, leftMargin, rightMargin, upMargin, lowMargin);

		//draw_line(leftMargin, upMargin, rightMargin, upMargin);
                //draw_line(leftMargin, upMargin, leftMargin, lowMargin);
                //draw_line(rightMargin, upMargin, rightMargin, lowMargin);

		display_image(img);

		queue_pop(queue);
		double *data = queue_pop(queue);
		for (size_t j = 0; j < 42; j++) {
			for (size_t i = 0; i < 42; i++) {
				printf("%d", (int)data[42*j + i + 1]);
			}
			printf("\n");
		}

                SDL_FreeSurface(img);
        }

        return (0);
}*/
