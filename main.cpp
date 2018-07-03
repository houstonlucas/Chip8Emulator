#include <iostream>
#include "Chip8Emulator.h"

int main() {

    Chip8Emulator emulator;

    std::string romPath = "15Puzzle.ch8";
    emulator.loadROM(romPath);
    emulator.runEmulation();


    return 0;
}