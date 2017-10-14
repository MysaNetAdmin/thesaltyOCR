# Simple Makefile

# Compilers vars
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -pedantic -std=c99 -O2
LDFLAGS=
LDLIBS= -lm

SRC= neuronet.c test.c
DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: test

test: neuronet.o test.o

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}

# END Makefile

