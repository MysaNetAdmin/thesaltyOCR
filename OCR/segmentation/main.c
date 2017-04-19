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
  SDL_FreeSurface(s);
  return 0;
}
