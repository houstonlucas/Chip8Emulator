#include <iostream>
#include "Chip8Emulator.h"

int main() {

    Chip8Emulator emulator;

    std::string romPath = "15Puzzle.ch8";

    if(!emulator.loadROM(romPath)){
        std::cout << "Bad ROM file." << std::endl;
        return -1;
    }

    emulator.runEmulation();

    return 0;
}