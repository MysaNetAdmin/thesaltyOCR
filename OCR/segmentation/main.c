#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"

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
  SDL_FreeSurface(res);
  return res;
}

SDL_Surface* horizon(SDL_Surface *img, size_t n){
/*  
  SDL_Surface *res = img;
  int tmp;
  size_t width = img->w;
  size_t height = img->h;
  size_t cpt;
  int boole;
  for(size_t i = 0; i < height; ++i){
    cpt = 0;
    tmp = 0;
    boole = 0;
    for(size_t j = 0; j < width; ++j){

      Uint32 pix = getpixel(img,i,j);
      if(pix == 16777215 && boole){
        cpt++;
      }
      else if(pix == 0){
        if(cpt <= n && boole){
          size_t k = tmp;
          while(j + cpt < width && k <= j + cpt)
          {
            putpixel(res,i,k,0);
            k++;
          }
          tmp = 0;
        }

        tmp = j;
        boole = 1;
      }
    }
  }
  return res;
  */
  SDL_Surface *res = img;
  size_t width = res->w;
  size_t heigt = res->h;
  int boo = 0;
  int cpt;
  int tmp;
  int track = 0;
  for(int i = 0;i < height;i++){
    for(int j = 0;j < width;j++){
      Uint32 pix = getpixel(img,i,j);
      if (track){
          if(pix == 0) {
            track = 0;
          }
          else{
            putpixel(res,i,j,8421504);
          }
      }
      else{
        if (boo && pix == 16777215){
            cpt++;
        }
        else if (!(cpt > n || cpt + j > width) && pix == 0){
            j = tmp;
            track = 1;
        }
        else{
          boo = 1;
          tmp = j;
        }
      }
    }
  }
  return res;
}

int main(){

  char *path = "syllabes-de-couleur.jpg";
  SDL_Surface* s = load_image(path);
  size_t width = s->w;
  size_t height = s->h;
  SDL_Surface *displayed = display_image(s);
  SDL_FreeSurface(displayed);
  for(size_t i = 0; i < width; ++i){
    for(size_t j = 0; j < height; ++j){

      Uint32 pix = getpixel(s,i,j);
      Uint8 r,g,b;
      SDL_GetRGB(pix,s->format, &r, &g, &b);
      r = 0.3 * r + 0.59 * g + 0.11 * b;
      if (r <= 123) r = 0;
      else r = 255;
      g = r;
      b = r;
      pix = SDL_MapRGB(s -> format,r,g,b);
      putpixel(s,i,j,pix);
    }
  }
  
  display_image(s);
  SDL_Surface *hori_dis = display_image(s);
  hori_dis = horizon(s,1);
  display_image(hori_dis);
  SDL_FreeSurface(hori_dis);
  return 0;
}
