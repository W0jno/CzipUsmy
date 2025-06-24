CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)


TARGET = chip8

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lSDL2

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ `sdl2-config --cflags --libs`


clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET) roms/PONG.ch8

.PHONY: all clean run
