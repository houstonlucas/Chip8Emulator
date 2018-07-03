//
// Created by namesnonames on 7/1/18.
//

#ifndef CHIP8EMULATOR_CPU_H
#define CHIP8EMULATOR_CPU_H


#include <stdint-gcc.h>

class CPU {
private:
    int programCounter;
    int stackPointer;


    uint16_t I;
    uint8_t V[16];
    uint8_t delayTimer;
    uint8_t soundTimer;

};


#endif //CHIP8EMULATOR_CPU_H
