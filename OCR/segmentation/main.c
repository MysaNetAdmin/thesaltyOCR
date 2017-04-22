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

SDL_Surface* horizon(SDL_Surface *img, size_t n){

  size_t width = img->w;
  size_t height = img->h;
  int boo = 0;
  size_t cpt;
  size_t tmp;
  for(size_t i = 0;i < height;i++){
    boo = 0;
    cpt = 0;
    tmp = 0;
    if(i % 100 == 0)
      printf("horizon -> height = %zu\n",i);
    for(size_t j = 0;j < width;j++){
      Uint32 pix = getpixel(img,j,i);
      Uint8 r,g,b;
      SDL_GetRGB(pix, img->format, &r, &g, &b);
      /*if(r < 123)
        boo = 1;
      if(r > 123 && boo){
        pix = SDL_MapRGB(img-> format , 122, 122, 122);
        putpixel(img,j,i,pix);
      }*/

      if(r < 122 && !boo){
        tmp = j;
        boo = 1;
      }
      else if(r > 122 && boo){
            cpt++;
      }
      else if(r < 122 && boo){
          if(cpt < n){
            while(tmp < j - 1){
              pix = SDL_MapRGB(img-> format , 0, 0, 0);
              putpixel(img,tmp+1,i,pix);
              tmp++;
            }        
          }
          else if(cpt < n * 3){
            while(tmp < j - 1){
              pix = SDL_MapRGB(img-> format , 122, 122, 122);
              putpixel(img,tmp+1,i,pix);
              tmp++;
            }       
          }

          cpt = 0;
	  tmp = j;
      }
    }
  }
  return img;
}

//same thing in vertical

SDL_Surface* vertical(SDL_Surface *img, size_t n){

  size_t width = img->w;
  size_t height = img->h;
  int boo = 0;
  size_t cpt;
  size_t tmp;
  for(size_t i = 0;i < width;i++){
    boo = 0;
    cpt = 0;
    tmp = 0;
    if(i % 100 == 0)
      printf("vertical -> width = %zu\n",i);
    for(size_t j = 0;j < height;j++){
      Uint32 pix = getpixel(img,i,j);
      Uint8 r,g,b;
      SDL_GetRGB(pix, img->format, &r, &g, &b);
      /*if(r < 123)
        boo = 1;
      if(r > 123 && boo){
        pix = SDL_MapRGB(img-> format , 122, 122, 122);
        putpixel(img,j,i,pix);
      }*/

     

      if(r < 122 && !boo){
        tmp = j;
        boo = 1;
      }
      else if(r > 122 && boo){
            cpt++;
      }
      else if(r < 122 && boo){
          if(cpt < n){
            while(tmp < j - 1){
              pix = SDL_MapRGB(img-> format , 0, 0, 0);
              putpixel(img,i,tmp + 1,pix);
              tmp++;
            }        
          }
          else if(cpt < n * 10){
            while(tmp < j - 1){
              pix = SDL_MapRGB(img-> format , 122, 122, 122);
              putpixel(img,i,tmp + 1,pix);
              tmp++;
            }       
          }

          cpt = 0;
	  tmp = j;
      }
    }
  }
  return img;
}

//create a new Surface which as been merged with the the two results of the two functions before

SDL_Surface* merge(SDL_Surface *img1, SDL_Surface *img2){

  size_t width = img2->w;
  size_t height = img2->h;
  for(size_t i = 0;i < width;i++){
    for(size_t j = 0;j < height;j++){
      Uint32 pix;
      Uint32 pix1 = getpixel(img1,i,j);
      Uint32 pix2 = getpixel(img2,i,j);

      Uint8 r1,g1,b1;
      Uint8 r2,g2,b2;

      SDL_GetRGB(pix1,img1->format,&r1,&g1,&b1);
      SDL_GetRGB(pix2,img2->format,&r2,&g2,&b2);

      if(r1 == 255 || r2 == 255){
          pix = SDL_MapRGB(img1->format,255,255,255);
      }
      else if(r1 == 122 || r2 == 122){
          pix = SDL_MapRGB(img1->format,122,122,122);
      }
      else{
          pix = SDL_MapRGB(img1->format,0,0,0);
      }
      putpixel(img1,i,j,pix);
    }
  }
  return img1;
}
//put in white each part which is not text
SDL_Surface* text(SDL_Surface *img){

    return img;
}
//separate each character with segment
SDL_Surface* grill(SDL_Surface *img){

  size_t width = img->w;
  size_t height = img->h;
  int boo = 0;
  size_t up = height;
  size_t down = 0;
  size_t right = 0;
  size_t left = width;
  for(size_t i = 0;i < height;i++){
    for(size_t j = 0;j < width;j++){
      Uint32 pix = getpixel(img,i,j);
      Uint8 r,g,b;
      SDL_GetRGB(pix,img->format,&r,&g,&b);
      if(r == 0){
        if (i < up)
          up = i;
        if (i > down)
          down = i;
        if (j < left)
          left = j;
        if (j > right)
          right = j;
        boo = 1;
      }
      
      }
    }
  return img;
}

//save each character in a 16x16 matrix 0 = white, 1 = black = letter

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
  SDL_Surface* verti_dis = display_image(ver);
  SDL_Surface* hori_dis = display_image(hor);
  SDL_Surface* merge_dis = display_image(ver);
  verti_dis = vertical(ver,3);
  hori_dis = horizon(hor,3);
  display_image(verti_dis);
  display_image(hori_dis);
  merge_dis = merge(verti_dis,hori_dis);
  display_image(merge_dis);
  return 0;
}
