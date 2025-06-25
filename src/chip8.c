#include "chip8.h"
#include <string.h>
#include <stdio.h>
#include "sdl.h"

uint8_t regiX;
uint8_t regiY;
uint8_t kkValue;
uint8_t result;
const uint8_t fonts[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *c){
    memset(c,0,sizeof(Chip8));
    c->pc=0x200;
    c->sp=0;
    memcpy(&c->memory[0x50], fonts, sizeof(fonts));
}

int chip8_load_rom(Chip8 *c, const char *path){
    FILE *rom = fopen(path, "rb");
    if (!rom) {
        puts("Failed to open ROM file");
        return -1;
    }

    fseek(rom, 0, SEEK_END);
    long size = ftell(rom);
    rewind(rom);
    
    if(size <= 0 || size > 3584){
        puts("Error: ROM size is invalid");
        fclose(rom);
        return -1;
    }

    size_t bytesRead = fread(&c->memory[0x200], 1, size, rom);
    
    fclose(rom);
    if (bytesRead != (size_t)size) {
        puts("Error: Failed to read the complete ROM file");
        return -1;
    }
    return 0;
}


void chip8_cycle(Chip8 *c){
    uint16_t opcode = c->memory[c->pc] << 8 | c->memory[c->pc+1];
    
    c->pc += 2;
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clears the screen        
                    
                break;
            
                case 0x000E: // 0x00EE: Returns from subroutine          
                    if (c->sp > 0) {
                        c->sp -= 1;
                        c->pc = c->stack[c->sp];
                    } else {
                        printf("Stack underflow on 0x00EE\n");
                    }
                break;
            }
        case 0x1000: // 0x1nnn sets the program counter to nnn
            c->pc = opcode & 0x0FFF;
            break;
        case 0x2000: // 0x2nnn The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            c->stack[c->sp] = c->pc;
            c->sp += 1;
            c->pc = opcode & 0x0FFF;
            break;
        case 0x3000: // 0x3xkk The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            regiX = (opcode & 0x0F00) >> 8;
            kkValue = opcode & 0x00FF;
            if(c->V[regiX] == kkValue){
                c->pc +=2;
            }
            break;
        case 0x4000: // 0x4xkk The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            regiX = (opcode & 0x0F00) >> 8;
            kkValue = opcode & 0x00FF;
            if(c->V[regiX] != kkValue){
                c->pc +=2;
            }
            break;
         case 0x5000: // 0x5xy0 The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            regiX = (opcode & 0x0F00) >> 8;
            regiY = (opcode & 0x00F0) >> 4;
            if(c->V[regiX] == c->V[regiY]){
                c->pc +=2;
            }
            break;
         case 0x6000: // 0x6xkk The interpreter puts the value kk into register Vx.
            regiX = (opcode & 0x0F00) >> 8;
            kkValue = opcode & 0x00FF;
            c->V[regiX] = kkValue;
            c->pc +=2;
            break;
        case 0x7000: // 0x7xkk Adds the value kk to the value of register Vx, then stores the result in Vx.
            regiX = (opcode & 0x0F00) >> 8;
            kkValue = opcode & 0x00FF;
            c->V[regiX] += kkValue;
            c->pc +=2;
            break;
        case 0x8000: 
            switch(opcode & 0x000F){
                case 0x0000: // 8xy0 Stores the value of register Vy in register Vx.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[regiX] = c->V[regiY];
                    c->pc +=2;
                    break;
                case 0x0001: // 8xy1 Set Vx = Vx OR Vy.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[regiX] = c->V[regiX] | c->V[regiY];
                    c->pc +=2;
                    break;
                case 0x0002: // 8xy2 Set Vx = Vx AND Vy.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[regiX] = c->V[regiX] & c->V[regiY];
                    c->pc +=2;
                    break;
                case 0x0003: // 8xy3 Set Vx = Vx XOR Vy.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[regiX] = c->V[regiX] ^ c->V[regiY];
                    c->pc +=2;
                    break;
                case 0x0004: // 8xy4 Set Vx = Vx + Vy, set VF = carry.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    uint16_t sum = c->V[regiX] + c->V[regiY];
                    c->V[0xF] = (sum > 0xFF) ? 1 : 0;
                    c->V[regiX] = sum & 0xFF;
                    c->pc +=2;
                    break;
                case 0x0005: // 8xy5 Set Vx = Vx - Vy, set VF = NOT borrow.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[0xF] = (c->V[regiX] > c->V[regiY]) ? 1 : 0;
                    uint16_t diff =  c->V[regiX] - c->V[regiY];
                    c->V[regiX] = diff & 0xFF;
                    c->pc +=2;
                    break;
                case 0x0006: // 8xy6 Set Vx = Vx SHR 1.

                    regiX = (opcode & 0x0F00) >> 8;
                    c->V[0xF] = c->V[regiX] & 0x1;
                    c->V[regiX] = c->V[regiX] >> 1;
                    c->pc +=2;
                    break;
                case 0x0007: // 8xy7 Set Vx = Vy - Vx, set VF = NOT borrow.
                    regiX = (opcode & 0x0F00) >> 8;
                    regiY = (opcode & 0x00F0) >> 4;
                    c->V[0xF] = (c->V[regiY] > c->V[regiX]) ? 1 : 0;
                    c->V[regiX] = (c->V[regiY] - c->V[regiX]) & 0xFF;
                    c->pc +=2;
                    break;
                case 0x000E: // 8xyE Set Vx = Vx SHL 1.
                    regiX = (opcode & 0x0F00) >> 8;
                    c->V[0xF] = c->V[regiX] & 0x1;
                    c->V[regiX] = c->V[regiX] << 1;
                    c->pc +=2;
                    break;
                default:
                    printf("\n Unknown opcode: 0x%X\n", opcode);
                    break;
            }   
            
        case 0x9000: // 0x9xy0 Skip next instruction if Vx != Vy.
            regiX = (opcode & 0x0F00) >> 8;
            regiY = (opcode & 0x00F0) >> 4;
            if(c->V[regiX] != c->V[regiY]){
                c->pc +=2;
            }
            break;
        case 0xA000: // 0xAnnn The value of register I is set to nnn.
            c->I = opcode & 0x0FFF;
            c->pc+=2;
            break;
        case 0xB000: // 0xBnnn Jump to location nnn + V0.
            c->pc = (opcode & 0x0FFF) + c->V[0];
            break;   
        default:
            printf("\n Unknown opcode: 0x%X\n", opcode);
            break;
    }

}