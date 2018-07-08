//
// Created by namesnonames on 7/1/18.
//

#ifndef CHIP8EMULATOR_CPU_H
#define CHIP8EMULATOR_CPU_H


#include <stdint-gcc.h>
#include "Screen.h"
#include "Buzzer.h"
#include "Chip8Emulator.h"

const uint8_t STACK_POINTER_BASE = 0x50;
const uint16_t PROGRAM_COUNTER_START = 0x200;

class Chip8Emulator;
class CPU {

private:
    uint16_t programCounter = PROGRAM_COUNTER_START;
    uint8_t stackPointer = STACK_POINTER_BASE;

    uint16_t I = 0;
    uint8_t V[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    uint8_t leftNibbleMask;
    uint8_t rightNibbleMask;

    Buzzer buzzer;

    void process0x8(uint8_t x, uint8_t y, uint8_t k);
    void process0xF(const Chip8Emulator *emulator, uint8_t x, uint8_t rightByte);

public:

    CPU();

    uint8_t getStackPointer() const;

    const int SPRITE_ADDRESSES[16] = {0x0, 0x5, 0xa, 0xf,
                                    0x14, 0x19, 0x1e, 0x23,
                                    0x28, 0x2d, 0x32, 0x37,
                                    0x3c, 0x41, 0x46, 0x4b
    };

    void processInstruction(const Chip8Emulator *emulator);

    void loadSpriteAddress(uint8_t x);

    void handleSound();

    uint16_t getProgramCounter() const;

    void decrementTimers();

    void waitForKey(const Chip8Emulator *emu, uint8_t x);

};


#endif //CHIP8EMULATOR_CPU_H
