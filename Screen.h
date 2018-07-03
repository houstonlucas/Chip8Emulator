//
// Created by namesnonames on 7/1/18.
//

#ifndef CHIP8EMULATOR_SCREEN_H
#define CHIP8EMULATOR_SCREEN_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string.h>

class Screen {
private:
    SDL_Window *window;
    int height;
    int width;

    const int numCellsWide = 64;
    const int numCellsTall = 32;

public:
    Screen(SDL_Window *window);
    void updateScreen(const uint8_t *vram);


};


#endif //CHIP8EMULATOR_SCREEN_H
