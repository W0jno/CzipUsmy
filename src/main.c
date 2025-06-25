#include <stdio.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

Chip8 myChip8;
int main(int argc, char **argv){
    chip8_init(&myChip8);
    sdl_init();
    chip8_load_rom(&myChip8, argv[1]);
    
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        sdl_draw(&myChip8);
        chip8_cycle(&myChip8);
        SDL_Delay(16);  // spowalniamy pętlę do ~60 FPS
    }
}