#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"

/* standard linked lists */
struct list {
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

SDL_Surface* init_img(SDL_Surface *img){

  SDL_Surface* res = SDL_CreateRGBSurface(0,img->w,img->h,32,255,255,255,255);
  for(int i = 0; i < img->w; i++){
    for(int j = 0; j < img->h; j++){
      Uint32 pix = getpixel(img,i,j);
      Uint8 r, g, b;
      SDL_GetRGB(pix, img -> format, &r, &g, &b);
      pix = SDL_MapRGB(img -> format, r, g, b);
      putpixel(res,i,j,pix);
    }
  }

  return res;
}

//undercircle the text in grey on horizontal and put everything which is not text in black

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
    display_image(img);
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
            putpixel(img,k - frontline,l,pix);
          }
        }
        display_image(inter);
        queue_push(queue,inter);

      }
    }
  }
  return queue;
}
//create a new Surface which as been merged with the the two results of the two functions before

SDL_Surface* merge(SDL_Surface *verti, SDL_Surface *hori){

  size_t width = verti->w;
  size_t height = verti->h;
  for(size_t i = 0;i < width;i++){
    for(size_t j = 0;j < height;j++){
      Uint32 pix;
      Uint32 pix_verti = getpixel(verti,i,j);
      Uint32 pix_hori = getpixel(hori,i,j);

      Uint8 r_verti,g_verti,b_verti;
      Uint8 r_hori,g_hori,b_hori;

      SDL_GetRGB(pix_verti,verti->format,&r_verti,&g_verti,&b_verti);
      SDL_GetRGB(pix_hori,hori->format,&r_hori,&g_hori,&b_hori);
      if(r_verti == 122 || r_hori == 122)
      {
        pix = SDL_MapRGB(verti -> format,122,122,122);
        putpixel(verti,i,j,pix);
      }
    }
  }
  return verti;
}
//put in white each part which is not text
SDL_Surface* text(SDL_Surface *img){

  return img;
}
//separate each character with segment
/*SDL_Surface* grill(SDL_Surface *img){

  size_t width = img->w;
  size_t height = img->h;

  size_t up ,down,left,right;
  up = height;
  down = 0;
  right = 0;
  left = width;

  for(size_t i = 0; i < height - 1;i++){
  for(size_t j = 0; j < width - 1;j++){

  Uint32 pix = getpixel(img,j,i)
  Uint8 r,g,b;
  SDL_GetRGB(pix,ver->format,&r,&g,&b);
  if(r == 0 || r == 122){
  if(j < left) left = j;
  if(j > right) right = j;
  if(i < up) up = i;
  if(i > down) down = i; 
  }
  if(r == 255){
//u && n
}

}
}

return img;
}*/

//save each character in a 16x16 matrix 0 = white, 1 = black = letter

SDL_Surface* compression(SDL_Surface* img){

  size_t width = img->w;
  size_t height = img->h;
  SDL_Surface* res = img;
  res->w = 16;
  res->h = 16;
  size_t w = width / 16;
  size_t h = height / 16;
  for(size_t i = 0;i < height;i++){
    for(size_t j = 0;j < width;j++){
      if(j % w == 0 && i % h == 0){
        Uint32 pix = getpixel(img,j,i);
        Uint8 r,g,b;
        SDL_GetRGB(pix,img->format,&r,&g,&b);
        pix = SDL_MapRGB(img->format,r,g,b);
        putpixel(img,j,i,pix);
      }
    }
  }
  return res;
}

int main(){

  char *path = "syllabes-de-couleur.jpg";
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
  //SDL_Surface* hori_dis = display_image(hor);
  //SDL_Surface* verti_dis = display_image(ver);
  //SDL_Surface* merge_dis = display_image(ver);
  struct queue *queue;
  queue = vertical(horizon(hor));
  //display_image(hori_dis);
  //verti_dis = vertical(hori_dis);
  //display_image(verti_dis);
  //merge_dis = merge(verti_dis,hori_dis);
  //display_image(merge_dis);
  return 0;
}
