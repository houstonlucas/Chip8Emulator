//
// Created by namesnonames on 7/2/18.
//

#ifndef CHIP8EMULATOR_CHIP8EMULATOR_H
#define CHIP8EMULATOR_CHIP8EMULATOR_H


#include "CPU.h"
#include "Screen.h"

class Chip8Emulator {
private:

    Screen *screen;
    CPU cpu;
    SDL_Window *window;
    SDL_GLContext context;
    const std::string windowTitle = "Chip8Emulator";

    const int HEIGHT = 320;
    const int WIDTH = 640;


public:
    ~Chip8Emulator();

    void runEmulation();

    bool setOpenGLAttributes();

    bool initMainWindow(const std::string &title);

    void checkSDLError(int line);

    void cleanupSDL();
};


#endif //CHIP8EMULATOR_CHIP8EMULATOR_H
