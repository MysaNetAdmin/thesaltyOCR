# Makefile
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0`
 
SRC= pixel_operations.c main.c interface.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
 
all: interface
 
debug: CFLAGS+= -O0 -g
debug: interface
 
clean:
	rm -f ${OBJ} ${DEP}
	rm -f interface
 
# END
