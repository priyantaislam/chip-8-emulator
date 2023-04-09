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
        break;
    }

}