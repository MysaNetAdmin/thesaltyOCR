#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include "pixel_operations.h"
#include "main.c"

const char usage[] =
"YOU FORGOT THE FUNCTION TO CALL\n"
"\t\t0: black and white\n"
"\t\t1: show black lines between lines\n"
"\t\t2: display lines of the image\n"
"\t\t3: display lines with black lines between letters\n"
"\t\t4: display the matrix of char\n";

int main(int argc,char *argv[]){
  if(argc <= 1)
    printf("you forgot your image\n");
  else if(argc <= 2)
		errx(1, "%s", usage);
	else{
		char *path = argv[1];
		unsigned int f = strtoul(argv[2], NULL, 20);
		SDL_Surface* ver = load_image(path);
		display_image(ver);
		//size_t width = ver->w;
		//size_t height = ver->h;
		struct queue* queue = malloc(sizeof(struct queue));
		switch(f){
			case 0:
				ver = black_n_white(ver);
				display_image(ver);
				break;
			case 1:
				trait_line(black_n_white(ver));
				break;
			case 2:
				queue = horizon(black_n_white(ver),1);
				free(queue);
				break;
			case 3:
				queue = trait_column(horizon(black_n_white(ver),0));
				break;
			case 4:
				queue = vertical(horizon(black_n_white(ver),0));
				while(!(queue_is_empty(queue)))
				{
					SDL_Surface* img = queue_pop(queue);
					print_matrix(matrix(img));
					printf("\n");
				}
		}
  }
  return 0;
}
 
