# Makefile
 
CC=gcc
 
CPPFLAGS= -MMD `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99 -O -lm -o
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image
 
SRC_MAIN= ../segmentation/pixel_operations.c ../segmentation/main.c ../xor/xor.c
OBJ_MAIN= ${SRC_MAIN:.c=}

SRC_INTERFACE= interface.c
OBJ_INTERFACE= ${SRC_INTERFACE:.c=.o}
 
all: ../segmentation/main interface ../xor/xor

main: ${OBJ_MAIN}

interface: ${OBJ_INTERFACE}
 
debug: CFLAGS+= -O0 -g
debug: main interface
 
clean:
	${RM} ../segmentation/main interface ../xor/xor
	${RM} ${OBJ_MAIN}
	${RM} ${OBJ_INTERFACE}
 
# END
