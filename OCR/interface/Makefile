# Makefile
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image
 
SRC_SEGMENTATION= ../segmentation/pixel_operations.c ../segmentation/main.c
OBJ_SEGMENTATION= ${SRC_SEGMENTATION:.c=.o}

SRC_INTERFACE= interface.c
OBJ_INTERFACE= ${SRC_INTERFACE:.c=.o}
 
all: segmentation interface

segmentation: ${OBJ_SEGMENTATION}

interface: ${OBJ_INTERFACE}
 
debug: CFLAGS+= -O0 -g
debug: segmentation interface
 
clean:
	${RM} segmentation interface
	${RM} ${OBJ_SEGMENTATION}
	${RM} ${OBJ_INTERFACE}
 
# END
