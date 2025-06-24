#include <stdbool.h>
#include <stdint.h>
typedef struct Chip8 { 
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer, sound_timer;
    bool gfx[64 * 32];
    bool key[16];
 } Chip8;

void chip8_init(Chip8 *c);
int  chip8_load_rom(Chip8 *c, const char *path);
void chip8_cycle(Chip8 *c);
void chip8_set_key(Chip8 *c, int key, bool pressed);