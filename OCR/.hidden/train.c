#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>
#include "NeuralNetwork/neuronet.h"
#include <string.h>
#include <sys/types.h>


#include "train.h"

char *conc(char *str1, char *str2, char *sep)
{
  char *res = calloc(strlen(str1) + strlen(str2) + 2, sizeof(char));
  strcpy(res, str1);
  strcpy(res + strlen(str1), sep);
  strcpy(res + strlen(str1) + 1, str2);
  return res;
}

char getres(size_t i)
{
  switch(i)
  {
  case 40:
    return 'a';
  case 31:
    return 'b';
  case 21:
    return 'c';
  case 12:
    return 'd';
  case 42:
    return 'e';
  case 43:
    return 'f';
  case 4:
    return 'g';
  case 8:
    return 'h';
  case 9:
    return 'i';
  case 17:
    return 'j';
  case 22:
    return 'k';
  case 7:
    return 'l';
  case 11:
    return 'm';
  case 1:
    return 'n';
  case 39:
    return 'o';
  case 16:
    return 'p';
  case 10:
    return 'q';
  case 18:
    return 'r';
  case 19:
    return 's';
  case 20:
    return 't';
  case 3:
    return 'u';
  case 13:
    return 'v';
  case 23:
    return 'w';
  case 24:
    return 'x';
  case 25:
    return 'y';
  case 28:
    return 'z';
  case 27:
    return 'A';
  case 26:
    return 'B';
  case 29:
    return 'C';
  case 30:
    return 'D';
  case 2:
    return 'E';
  case 32:
    return 'F';
  case 33:
    return 'G';
  case 34:
    return 'H';
  case 35:
    return 'I';
  case 36:
    return 'J';
  case 37:
    return 'K';
  case 38:
    return 'L';
  case 15:
    return 'M';
  case 14:
    return 'N';
  case 41:
    return 'O';
  case 5:
    return 'P';
  case 6:
    return 'Q';
  case 44:
    return 'R';
  case 45:
    return 'S';
  case 46:
    return 'T';
  case 47:
    return 'U';
  case 48:
    return 'V';
  case 49:
    return 'W';
  case 50:
    return 'X';
  case 51:
    return 'Y';
  case 52:
    return 'Z';
  default:
    return ' ';
  }
}

double *getoutput(char *symbole)
{
  double *res = calloc(53, sizeof(double));
  res[0] = 1;
  switch(*symbole)
  {
  case 'a':
    res[1] = 1;
    return res;
  case 'b':
    res[2] = 1;
    return res;
  case 'c':
    res[3] = 1;
    return res;
  case 'd':
    res[4] = 1;
    return res;
  case 'e':
    res[5] = 1;
    return res;
  case 'f':
    res[6] = 1;
    return res;
  case 'g':
    res[7] = 1;
    return res;
  case 'h':
    res[8] = 1;
    return res;
  case 'i':
    res[9] = 1;
    return res;
  case 'j':
    res[10] = 1;
    return res;
  case 'k':
    res[11] = 1;
    return res;
  case 'l':
    res[12] = 1;
    return res;
  case 'm':
    res[13] = 1;
    return res;
  case 'n':
    res[14] = 1;
    return res;
  case 'o':
    res[15] = 1;
    return res;
  case 'p':
    res[16] = 1;
    return res;
  case 'q':
    res[17] = 1;
    return res;
  case 'r':
    res[18] = 1;
    return res;
  case 's':
    res[19] = 1;
    return res;
  case 't':
    res[20] = 1;
    return res;
  case 'u':
    res[21] = 1;
    return res;
  case 'v':
    res[22] = 1;
    return res;
  case 'w':
    res[23] = 1;
    return res;
  case 'x':
    res[24] = 1;
    return res;
  case 'y':
    res[25] = 1;
    return res;
  case 'z':
    res[26] = 1;
    return res;
  case 'A':
    res[27] = 1;
    return res;
  case 'B':
    res[28] = 1;
    return res;
  case 'C':
    res[29] = 1;
    return res;
  case 'D':
    res[30] = 1;
    return res;
  case 'E':
    res[31] = 1;
    return res;
  case 'F':
    res[32] = 1;
    return res;
  case 'G':
    res[33] = 1;
    return res;
  case 'H':
    res[34] = 1;
    return res;
  case 'I':
    res[35] = 1;
    return res;
  case 'J':
    res[36] = 1;
    return res;
  case 'K':
    res[37] = 1;
    return res;
  case 'L':
    res[38] = 1;
    return res;
  case 'M':
    res[39] = 1;
    return res;
  case 'N':
    res[40] = 1;
    return res;
  case 'O':
    res[41] = 1;
    return res;
  case 'P':
    res[42] = 1;
    return res;
  case 'Q':
    res[43] = 1;
    return res;
  case 'R':
    res[44] = 1;
    return res;
  case 'S':
    res[45] = 1;
    return res;
  case 'T':
    res[46] = 1;
    return res;
  case 'U':
    res[47] = 1;
    return res;
  case 'V':
    res[48] = 1;
    return res;
  case 'W':
    res[49] = 1;
    return res;
  case 'X':
    res[50] = 1;
    return res;
  case 'Y':
    res[51] = 1;
    return res;
  case 'Z':
    res[52] = 1;
    return res;
  default:
    return res;
  }
}

void parcour(char* Dir, size_t iteration)
{
   // size_t layers[3] = {401, 1000, 53};
    //struct net *net = makeNet(3, layers);
    struct net *net = loadNet("OCR.bin");

    DIR *samples = opendir(Dir);
    struct dirent *underDir = readdir(samples);
    for(size_t i = 0; i < iteration; i++)
    {
        while(underDir)
        {
            if(strcmp(underDir->d_name, ".") && strcmp(underDir->d_name, ".."))
            {
              char *letterfold = conc(Dir, underDir->d_name, "/");
              DIR *fold = opendir(letterfold);
if(fold){
              struct dirent *file = readdir(fold);
              while(file)
              {
                if(strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
                {
                  char *dataname = conc(letterfold, file->d_name, "/");
                  printf("%s\n", dataname);
                  double *data = convertfromexemples(dataname);
                  back(net, data, getoutput(underDir->d_name), 0.4);
                  free(data);
                }
                file = readdir(fold);
              }
              closedir(fold);
}
            }
            underDir = readdir(samples);
        }
        rewinddir(samples);
        underDir = readdir(samples);
        saveNet(net, "OCR.bin");
    }
    closedir(samples);
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

static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}



double *convertfromexemples(char *path)
{
  init_sdl();
  SDL_Surface* img = load_image(path);
  double *res = malloc(sizeof(double) * 401);
  res[0] = 1;
  for(size_t i = 0; i < 400; i++)
  {
    Uint32 pix = getpixel(img, i%20, i/20);
    Uint8 r, g, b;
    SDL_GetRGB(pix, img->format, &r, &g, &b);
    double pixel = (double)r / 255 > 0.5 ?  0 : 1;
    res[i + 1] = pixel;
  }
  SDL_FreeSurface(img);
  return res;
}

char OCR(struct net *net, double *inputs)
{
  size_t r = compute(net, inputs);
  return getres(r);
}

int main(int argv, char* argc[])
{
// parcour("training",50);
  struct net *net = loadNet("OCR.bin");
  //char *train = "training/z/0.gif";
 // parcour(train, 100);
  printf("%c\n",OCR(net, convertfromexemples(argc[1])));
  return 0;
}
