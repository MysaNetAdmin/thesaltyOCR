CC= gcc
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= pixel_operations.c queue.c main.c
OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main
