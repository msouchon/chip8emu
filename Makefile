OBJS = main.c chip8.c opcodes.c graphics.c font.c

CC = gcc

INCLUDE_PATHS = -I C:\msys64\mingw64\include\SDL2

LIBRARY_PATHS = -L C:\msys64\mingw64\lib

COMPILER_FLAGS = -Wall

LINKER_FLAGS = -l mingw32 -l SDL2main -l SDL2 -l SDL2_ttf

OBJ_NAME = main.exe

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
