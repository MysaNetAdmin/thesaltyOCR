# Makefile
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image
 
SRC_MAIN= pixel_operations.c main.c
OBJ_MAIN= ${SRC_MAIN:.c=.o}

SRC_INTERFACE= interface.c
OBJ_INTERFACE= ${SRC_INTERFACE:.c=.o}
 
all: main interface

main: ${OBJ_MAIN}

interface: ${OBJ_INTERFACE}
 
debug: CFLAGS+= -O0 -g
debug: main interface
 
clean:
	${RM} main interface
	${RM} ${OBJ_MAIN}
	${RM} ${OBJ_INTERFACE}
 
# END
