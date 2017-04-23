# Makefile
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0`
 
SRC= interface.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
 
all: interface
 
main: ${OBJ}
 
debug: CFLAGS+= -O0 -g
debug: main
 
clean:
	rm -f ${OBJ} ${DEP}
	rm -f main
 
# END
