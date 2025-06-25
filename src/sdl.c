#include "SDL2/SDL.h"
#include "chip8.h"
#include "sdl.h"
#define SCALE 10

SdlState mySdlState;

void sdl_init(){
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    mySdlState.window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        64*SCALE, 32*SCALE,
        SDL_WINDOW_SHOWN
    );

    mySdlState.renderer = SDL_CreateRenderer(mySdlState.window, -1, SDL_RENDERER_ACCELERATED);
}

void sdl_draw(Chip8 *c){
    
    SDL_SetRenderDrawColor(mySdlState.renderer, 255, 255, 255, 255);
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 64; j++){
            if (c->gfx[i * 64 + j]) {
                SDL_Rect rect = { j * SCALE, i * SCALE, SCALE, SCALE };
                SDL_RenderFillRect(mySdlState.renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(mySdlState.renderer);
}