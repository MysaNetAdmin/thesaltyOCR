C=gcc
CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99 -O3
LDLIBS= `pkg-config --libs sdl gtk+-3.0 gmodule-2.0` -lSDL_image -lm -export-dynamic
EXEC= interface
 
all: $(EXEC)
 
main: interface.o
	$(CC) -o $@ $^ $(LDLIBS)
clean:
	rm -rf *.o *.save *~ *.bak *.swp $(EXEC)
