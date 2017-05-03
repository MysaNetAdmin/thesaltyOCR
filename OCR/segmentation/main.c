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


void init_sdl(void) {
  if(SDL_Init(SDL_INIT_VIDEO) == -1){

    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }

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
  size_t frontline,backline;;
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


struct queue* horizon(SDL_Surface *img){

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

struct queue* fill(struct queue* queue){

  struct queue *res;
  res = malloc(sizeof(struct queue));
  res->store = NULL;
  res->size = 0;
  

  while(!queue_is_empty(queue)){

    SDL_Surface* tmp = queue_pop(queue);
    size_t width = tmp->w;
    size_t height = tmp->h;
    
    SDL_Surface* inter = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);

    for(size_t i = 0;i < 16;i++){
      for(size_t j = 0;j < 16;j++){
        if(i < width && j < height){
          Uint32 pix = getpixel(tmp,i,j);
          putpixel(inter,i,j,pix);
        }
        else{
          Uint32 pix = SDL_MapRGB(inter->format,255,255,255);
          putpixel(inter,i,j,pix);
        }
      }
    }

    queue_push(res,inter);
  }
  free(queue);
  return res;
}

//return a double-dimension array fill with 0 and 1 (0 == white pixel and 1 == black pixel) 

int** matrix (SDL_Surface* img){

  size_t width = img->w;
  size_t height = img->h;

  int** res = malloc(sizeof *res *16);
  for(size_t k = 0;k < 16;k++){
    res[k] = malloc(sizeof **res * 16);
  }

  for(size_t i = 0;i < height;i++){
    for(size_t j = 0;j < width;j++){

        Uint32 pix = getpixel(img,j,i);
        Uint8 r,g,b;
        SDL_GetRGB(pix,img->format,&r,&g,&b);

        if(r < 122){
          res[i][j] = 7;
        }
        else{
          res[i][j] = 0;
        }
    }
  }
  return res;
}

void print_matrix(int** matrix){

  for(int i = 0;i < 16;i++){
    for(int j = 0;j < 16;j++){
      printf("%d ",matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc,char *argv[]){
  
  if(argc <= 1)
    printf("you forget your image");
  else{
  char *path = argv[1];
  SDL_Surface* ver = load_image(path);
  SDL_Surface* hor = load_image(path);
  size_t width = ver->w;
  size_t height = ver->h;
  printf("%zu\n",width);
  printf("%zu\n",height);
  SDL_Surface *displayed = display_image(ver);
  SDL_FreeSurface(displayed);
  for(size_t i = 0; i < width; ++i){
    for(size_t j = 0; j < height; ++j){

      Uint32 pix = getpixel(ver,i,j);
      Uint8 r,g,b;
      SDL_GetRGB(pix,ver->format, &r, &g, &b);
      r = 0.3 * r + 0.59 * g + 0.11 * b;
      if (r <= 123) r = 0;
      else r = 255;
      g = r;
      b = r;
      pix = SDL_MapRGB(ver -> format,r,g,b);
      putpixel(ver,i,j,pix);
      putpixel(hor,i,j,pix);
    }
  }
  display_image(ver);

  //trait_line(ver);
  struct queue *final;
  final = fill(vertical(horizon(hor)));
  struct queue *res;
  res = malloc(sizeof(struct queue));
  res->store = NULL;
  res->size = 0;

  printf("%zu\n",res->size);
  int i = 0;
  int** inter;
  while(!queue_is_empty(final)){
      inter = matrix(queue_pop(final));
      queue_push(res, inter);
      print_matrix(inter);
      printf("\n");
    i++;
  }
}
  return 0;
}
