FLAGS = -Wall -Werror -g
SDL = -lSDL2 `sdl2-config --libs --cflags`
INCLUDE = -I ./include
FILES = ./src/*

EXECUTABLE = gameboy

all: 
	gcc ${FLAGS} ${INCLUDE} ${SDL} ${FILES} -o ./${EXECUTABLE}