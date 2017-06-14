#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"


struct list{
  struct list *next;
  void        *data;
};

/*
 * queue container: replace sentinel and add abstraction
 */
struct queue {
  struct list *store;
  size_t       size;
};

void queue_init(struct queue *queue){
  queue->size = 0;
  queue->store = NULL;
}

/*
 * queue_is_empty(queue) test for emptyness
 */
int queue_is_empty(struct queue *queue) {

  if (queue->size == 0) {
    return (1);
  } else {
    return (0);
  }
}

/*
 * queue_push(queue, elm) push elm
 */
void queue_push(struct queue *queue, void *elm) {

  struct list* node = malloc(sizeof (struct list));
  node->next = (queue_is_empty(queue) ? node : queue->store->next);
  node->data = elm;

  if (!queue_is_empty(queue)) {
    queue->store->next = node;
  }
  queue->store = node;
  queue->size += 1;
}




/*
 * queue_pop(queue) pop the next element (FIFO order)
 * returns NULL if the queue is empty
 */
void* queue_pop(struct queue *queue) {

  if (queue_is_empty(queue)) {
    return (NULL);
  } else {
    struct list* tmp = queue->store->next;
    queue->store->next = queue->store->next->next;
    void* elm = tmp->data;
    //free(tmp);
    queue->size -= 1;
    return (elm);
  }
}


void wait_for_keypressed(void){
  SDL_Event             event;
  for(;;){
    SDL_PollEvent( &event );
    switch (event.type){

      case SDL_KEYDOWN: return;
      default: break;
    }
  }
}


SDL_Surface* init_SDL(size_t height, size_t width) {
  SDL_Surface *img = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
  for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < width; j++){
      Uint32 pix = SDL_MapRGB(img -> format,0,0,0);
      putpixel(img,j,i,pix);
    }
  }
  return img;

}

SDL_Surface* load_image(char *path){
  SDL_Surface           *img;
  img = IMG_Load(path);
  if (!img) errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface             *screen;

  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if(screen == NULL) {
    errx(1, "Couldn't  set %dx%d video mode: %s\n",
        img->w, img->h, SDL_GetError());
  }

  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());

  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
  wait_for_keypressed();

  return screen;
}


void trait_line(SDL_Surface *img)

{
  size_t width = img->w;
  size_t height = img->h;
  int boo = 0;
 size_t cpt;
  size_t frontline,backline;
  for(size_t i = 0;i < height;i++){
    cpt = 0;
    for(size_t j = 0;j < width;j++){
      Uint32 pix = getpixel(img,j,i);
      Uint8 r,g,b;
      SDL_GetRGB(pix, img->format, &r, &g, &b);

      if(r < 122 && !boo){
        frontline = i;
        boo = 1;
      }
      else if(r < 122 && boo){
        cpt++;
      }
    }
    if(boo && cpt == 0){
      backline = i;
      boo = 0; 
      for(size_t k = 0 ;k < width;k++){
        Uint32 pix = SDL_MapRGB(img->format,0,0,0);
        putpixel(img,k,frontline - 1,pix);
        putpixel(img,k,backline ,pix);
      }

    }
  }
  display_image(img);
}

struct queue* trait_column(struct queue* queue){
  struct queue* res = malloc(sizeof(struct queue));
  queue_init(res);
  while(!(queue_is_empty(queue)))
  {
    SDL_Surface *img = queue_pop(queue);
    size_t width = img->w;
    size_t height = img->h;
    int boo = 0;
    size_t cpt;
    size_t frontline,backline;
    for(size_t i = 0;i < width;i++){
      cpt = 0;
      for(size_t j = 0;j < height;j++){
        Uint32 pix = getpixel(img,i,j);
        Uint8 r,g,b;
        SDL_GetRGB(pix, img->format, &r, &g, &b);

        if(r < 122 && !boo){
          frontline = i;
          boo = 1;
        }
        else if(r < 122 && boo){
          cpt++;
        }
      }
      if(boo && cpt == 0){
        backline = i;
        boo = 0; 
        for(size_t k = 0 ;k < height;k++){
          Uint32 pix = SDL_MapRGB(img->format,0,0,0);
          putpixel(img,frontline,k,pix);
          putpixel(img,backline,k ,pix);
        }
      }
    }
    display_image(img);
    //queue_push(res,img);
  }
  return res;
}
struct queue* horizon(SDL_Surface *img, int print){

  struct queue *queue;
  queue = malloc(sizeof(struct queue));
  queue->store = NULL;
  queue->size = 0;

  size_t width = img->w;
  size_t height = img->h;
  int boo = 0;
  size_t cpt;
  size_t frontline,backline;

  for(size_t i = 0;i < height;i++){
    cpt = 0;
    for(size_t j = 0;j < width;j++){
      Uint32 pix = getpixel(img,j,i);
      Uint8 r,g,b;
      SDL_GetRGB(pix, img->format, &r, &g, &b);

      if(r < 122 && !boo){
        frontline = i;
        boo = 1;
      }
      else if(r < 122 && boo){
        cpt++;
      }
    }
    if(boo && cpt == 0){
      backline = i;
      boo = 0;
      SDL_Surface* inter = SDL_CreateRGBSurface(0,width,backline - frontline,32,0,0,0,0);
      for(size_t k = frontline; k < backline; k++)
      {
        for(size_t l = 0; l < width ;l++)
        {
          Uint32 pix = getpixel(img,l,k);
          putpixel(inter,l,k - frontline,pix);
        }
      }
      if(print)
        display_image(inter);
      queue_push(queue,inter);
    }
  }
  return queue;
}
//same thing in vertical

struct queue* vertical(struct queue *kebab)
{
  struct queue *queue;
  queue = malloc(sizeof(struct queue));
  queue->store = NULL;
  queue->size = 0;

  while(!queue_is_empty(kebab))
  {
    SDL_Surface* img = queue_pop(kebab);
    size_t width = img->w;
    size_t height = img->h;
    int boo = 0;
    size_t cpt;
    size_t frontline,backline;
    for(size_t i = 0;i < width;i++){
      cpt = 0;
      for(size_t j = 0;j < height;j++){
        Uint32 pix = getpixel(img,i,j);
        Uint8 r,g,b;
        SDL_GetRGB(pix, img->format, &r, &g, &b);

        if(r < 122 && !boo){
          frontline = i;
          boo = 1;
        }
        else if(r < 122 && boo){
          cpt++;
        }
      }
      if(boo && cpt == 0){
        backline = i;
        boo = 0;
        SDL_Surface* inter = SDL_CreateRGBSurface(0,backline - frontline,height,32,0,0,0,0);
        for(size_t k = frontline; k < backline; k++)
        {
          for(size_t l = 0; l < height ;l++)
          {
            Uint32 pix = getpixel(img,k,l);
            putpixel(inter,k - frontline,l,pix);
          }
        }
        queue_push(queue,inter);
      }
    }
  }
  free(kebab);
  return queue;
}


//put in white each part which is not text
SDL_Surface* text(SDL_Surface *img){

  return img;
}


//fill each SDL element with white pixel until the size reach 16x16

int** fill(SDL_Surface* tmp){

  int** res = malloc(sizeof(int) * 16);
  for(size_t k = 0;k < 16;k++){
    res[k] = malloc(sizeof(int) * 16);
  }
  size_t width = tmp->w;
  size_t height = tmp->h;

  for(size_t i = 0;i < height;i++){
    for(size_t j = 0;j < width;j++){
        Uint32 pix = getpixel(tmp,j,i);
        Uint8 r,g,b;
        SDL_GetRGB(pix,tmp->format,&r,&g,&b);
        if(r < 122) res[i][j] =  1;
        else  res[i][j] = 0;
      }
    }
  return res;
}

SDL_Surface* resize(SDL_Surface* tmp){
  
  SDL_Surface* res = init_SDL(16,16);
  size_t height = tmp->h;
  size_t width = tmp->w;
  
  size_t x,y;

  Uint8 A,B,C,grey;

  float x_ratio = ((float)(width - 1))/16;
  float y_ratio = ((float)(height-1))/16;
  float x_diff,y_diff;
  size_t offset = 0;
  for(size_t i = 0;i < 16;i++){
    for(size_t j = 0; j < 16;j++){
      x = (size_t)(x_ratio * j);
      y = (size_t)(y_ratio * i);
      x_diff = (x_ratio * j) - x;
      y_diff = (y_ratio * i) - y;
      

      A = getpixel(tmp,x,y) & 0xff;
      B = getpixel(tmp,x,y + 1) & 0xff;
      C = getpixel(tmp,x,y + width) & 0xff;

      if(A + B + C < 255) grey = 0;
      else  grey = 255;

      Uint32 pix = SDL_MapRGB(tmp->format,grey,grey,grey);
      putpixel(res,offset++,y,pix);
    }
  }
  return res;
}

//return a double-dimension array fill with 0 and 1 (0 == white pixel and 1 == black pixel) 

int** matrix (SDL_Surface* img){

  size_t width = img->w;
  size_t height = img->h;
  
  int** res;

  if(width > 16 || height > 16){

    res = malloc(sizeof *res * 16);
    for(size_t k = 0; k < 16; k++){
      res[k] = malloc(sizeof(**res * 16));
    }

    img = resize(img);
    for(size_t i = 0; i < height;i++){
      for(size_t j = 0; j < width;j++){
        Uint32 pix = getpixel(img,j,i);
        Uint8 r,g,b;
        SDL_GetRGB(pix,img->format,&r,&g,&b);
        if(r < 127) res[i][j] = 1;
        else  res[i][j] = 0;
      }
    }
    return res;
  }
  
  else  {
    res = fill(img);
  }
  return res; 
}

void print_matrix(int** matrix){
  for(int i = 0;i < 16;i++){
    for(int j = 0;j < 16;j++){
      if (matrix[i][j] == 1) printf("%c[1;31m%d ",27,matrix[i][j]);
      else printf("%c[1;32m%d ", 27,matrix[i][j]); 
      //printf("%d ",matrix[i][j]);
    }
    printf("\n");
  }
}

SDL_Surface* black_n_white(SDL_Surface* img){
  size_t width = img->w;
  size_t height = img->h;
  for(size_t i = 0; i < width; ++i){
    for(size_t j = 0; j < height; ++j){

      Uint32 pix = getpixel(img,i,j);
      Uint8 r,g,b;
      SDL_GetRGB(pix,img->format, &r, &g, &b);
      r = 0.3 * r + 0.59 * g + 0.11 * b;
      if (r <= 123) r = 0;
      else r = 255;
      g = r;
      b = r;
      pix = SDL_MapRGB(img -> format,r,g,b);
      putpixel(img,i,j,pix);
    }
  }
  return img;
}

/* FONCTIONS A UTILISER */

void display_black_n_white(SDL_Surface *img)
{
  SDL_Surface* ver = black_n_white(img);
	display_image(ver);
}

void display_line(SDL_Surface *img)
{
  trait_line(black_n_white(img));
}

void display_column(SDL_Surface *img)
{
	struct queue* queue = trait_column(horizon(black_n_white(img),0));
	free(queue);
}

void display_mat(SDL_Surface *img)
{
	struct queue* queue = vertical(horizon(black_n_white(img),0));
  while(!(queue_is_empty(queue)))
  {
		printf("oui\n");
    SDL_Surface* img2 = queue_pop(queue);
    print_matrix(matrix(img2));
    printf("\n");
  } 
}


const char usage[] =
"YOU FORGET THE FUNCTION TO CALL\n"
"\t\t0: black and white\n"
"\t\t1: show black lines between lines\n"
"\t\t2: display lines of the image\n"
"\t\t3: display lines with black lines between letters\n"
"\t\t4: display the matrix of char\n";

int main(int argc,char *argv[]){
	if(argc <= 1)
		printf("you forget your image\n");
	else if(argc <= 2)
		errx(1, "%s", usage);
	else{
		char *path = argv[1];
		unsigned int f = strtoul(argv[2], NULL, 20);
		SDL_Surface* ver = load_image(path);
		//display_image(ver);
		//size_t width = ver->w;
		//size_t height = ver->h;
		switch(f){
			case 0:
				display_black_n_white(ver);
				break;
			case 1:
				display_line(ver);
				break;
			case 2:
				display_column(ver);
				break;
			case 3:
				display_mat(ver);
				break;
		}
	}
	return 0;
}
