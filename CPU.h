//
// Created by namesnonames on 7/1/18.
//

#ifndef CHIP8EMULATOR_CPU_H
#define CHIP8EMULATOR_CPU_H


#include <stdint-gcc.h>
#include "Screen.h"

class CPU {
private:
    int stackPointer;
    int programCounter = 0x200;


    uint16_t I;
    uint8_t V[16];
    uint8_t delayTimer;
    uint8_t soundTimer;

    void process0x8(uint8_t x, uint8_t y, uint8_t k);
    void process0xF(uint8_t *memory, uint8_t x, uint8_t rightByte);

public:

    const int SPRITE_ADDRESSES[16] = {0x0, 0x5, 0xa, 0xf,
                                    0x14, 0x19, 0x1e, 0x23,
                                    0x28, 0x2d, 0x32, 0x37,
                                    0x3c, 0x41, 0x46, 0x4b
    };
    void processInstruction(uint8_t *memory, Screen *screen);

    void loadSpriteAddress(uint8_t x);

    int getProgramCounter() const;
};


#endif //CHIP8EMULATOR_CPU_H
