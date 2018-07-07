//
// Created by namesnonames on 7/1/18.
//

#ifndef CHIP8EMULATOR_CPU_H
#define CHIP8EMULATOR_CPU_H


#include <stdint-gcc.h>
#include "Screen.h"
const uint8_t STACK_POINTER_BASE = 0x50;
const uint16_t PROGRAM_COUNTER_START = 0x200;

class CPU {

private:
    uint16_t programCounter = PROGRAM_COUNTER_START;
    uint8_t stackPointer = STACK_POINTER_BASE;

    uint16_t I = 0;
    uint8_t V[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    const uint8_t leftNibbleMask = 0xf0;
    const uint8_t rightNibbleMask = 0x0f;

    void process0x8(uint8_t x, uint8_t y, uint8_t k);
    void process0xF(uint8_t *memory, uint8_t x, uint8_t rightByte);

public:
    uint8_t getStackPointer() const;

    const int SPRITE_ADDRESSES[16] = {0x0, 0x5, 0xa, 0xf,
                                    0x14, 0x19, 0x1e, 0x23,
                                    0x28, 0x2d, 0x32, 0x37,
                                    0x3c, 0x41, 0x46, 0x4b
    };
    void processInstruction(uint8_t *memory, Screen *screen);

    void loadSpriteAddress(uint8_t x);

    uint16_t getProgramCounter() const;

    void decrementTimers();
};


#endif //CHIP8EMULATOR_CPU_H
