//
// Created by namesnonames on 7/2/18.
//

#ifndef CHIP8EMULATOR_CHIP8EMULATOR_H
#define CHIP8EMULATOR_CHIP8EMULATOR_H


#include "CPU.h"
#include "Screen.h"
#include <fstream>
#include <map>


class CPU;
class Chip8Emulator {
private:
    SDL_Window *window;
    SDL_GLContext context;
    const std::string windowTitle = "Chip8Emulator";

    const int HEIGHT = 320;
    const int WIDTH = 640;


public:
    Screen *screen;
    CPU *cpu;
    bool inputValues[16];
    std::map<SDL_Keycode, uint8_t> keyMapping;
    uint8_t *memory;

    Chip8Emulator();

    ~Chip8Emulator();

    void runEmulation();

    bool setOpenGLAttributes();

    bool initMainWindow(const std::string &title);

    void checkSDLError(int line);

    void cleanupSDL();

    bool loadROM(std::string romPath);

    void initializeSpriteMemory();

    void initializeKeyMapping();

    bool updateInputValues(SDL_Event &event);
};


#endif //CHIP8EMULATOR_CHIP8EMULATOR_H
