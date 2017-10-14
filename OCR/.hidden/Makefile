# Makefile
 
CC=gcc
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl` -lSDL_image
 
SRC= train.c NeuralNetwork/neuronet.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
 
all: train

train: ${OBJ}
 
-include ${DEP}
 
main_tests: ${OBJ}
 
clean:
	rm -f ${OBJ} ${DEP} train
# END
