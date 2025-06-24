#include <stdio.h>
#include <chip8.h>

Chip8 myChip8;
int main(int argc, int **argv){
    chip8_init(&myChip8);
    chip8_load_rom(&myChip8, argv[1]);
    
    return 0;
}