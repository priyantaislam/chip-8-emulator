#include "chip8.h"
#include <memory.h>
#include <assert.h>

const char chip8_default_character_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,   //0
    0x20, 0x60, 0x20, 0x20, 0x70,   //1
    0xf0, 0x10, 0xf0, 0x80, 0xf0,   //2
    0xf0, 0x10, 0xf0, 0x10, 0xf0,   //3
    0x90, 0x90, 0xf0, 0x10, 0x10,   //4
    0xf0, 0x80, 0xf0, 0x10, 0xf0,   //5
    0xf0, 0x80, 0xf0, 0x90, 0xf0,   //6
    0xf0, 0x10, 0x20, 0x40, 0x40,   //7
    0xf0, 0x90, 0xf0, 0x90, 0xf0,   //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,   //9
    0xF0, 0x90, 0xF0, 0x90, 0x90,   //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,   //B
    0xF0, 0x80, 0x80, 0x80, 0xF0,   //C
    0xE0, 0x90, 0x90, 0x90, 0xE0,   //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,   //E
    0xF0, 0x80, 0xF0, 0x80 ,0x80    //F

};
void chip8_init(struct chip8* chip8) {
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct chip8* chip8, const char* buf, size_t size){

    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;

}

static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode) {
    
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    switch(opcode & 0xf000){
        //JP addr, 1nnn Jump to location nnn
        case 0x1000:
            chip8->registers.PC = nnn;
            break;
        //CALL addr, 2nnn call subroutine at location nnn
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
            break;
        // SE Vx, byte- 3xkk Skip next instruction if Vx=kk
        case 0x3000:
            if(chip8->registers.V[x] == kk){
                chip8->registers.PC += 2;
            }
            break;
        // SNE Vx, byte- 4xkk Skip next instruction if Vx!=kk
        case 0x4000:
            if(chip8->registers.V[x] != kk){
                chip8->registers.PC += 2;
            }
            break;
        //5xy0 - SE, Vx, Vy, skip the next instruction if Vx = Vy
        case 0x5000:
            if (chip8->registers.V[x] == chip8->registers.V[y]){
                chip8->registers.PC += 2;
            }
            break;
        //LD Vx, byte- Vx = KK
        case 0x6000:
            chip8->registers.V[x] = kk;
            break;
        
        //7xkk - ADD Vx, byte - Set Vx = Vx + kk
        case 0x7000:
            chip8->registers.V[x] += kk;
            break;
    }
}


void chip8_exec(struct chip8* chip8, unsigned short opcode) {

    switch (opcode){
    //clear the display
    case 0x00E0:
        chip8_screen_clear(&chip8->screen);
        break;
    //returns from subroutine
    case 0x00EE:
        chip8->registers.PC = chip8_stack_pop(chip8);
    default:
        chip8_exec_extended(chip8, opcode);
        break;
    }

}