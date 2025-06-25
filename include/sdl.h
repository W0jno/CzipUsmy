#include "SDL2/SDL.h"
#include "chip8.h"
typedef struct SdlState{
    SDL_Window *window;
    SDL_Renderer *renderer;
} SdlState;

void sdl_init();
void sdl_draw(Chip8 *c);