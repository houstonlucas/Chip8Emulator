//
// Created by namesnonames on 7/7/18.
//

#ifndef CHIP8EMULATOR_BUZZER_H
#define CHIP8EMULATOR_BUZZER_H

#include <stdint-gcc.h>
#include <SDL2/SDL.h>
#include <math.h>

const int AMPLITUDE = 400;
const int SAMPLE_RATE = 44100;


class Buzzer {
public:

    void startBeep();
    void stopBeep();
    bool isPlaying = false;

    int sample_nr = 0;
    SDL_AudioSpec want;
    SDL_AudioSpec have;
    Buzzer();
    ~Buzzer();

};


#endif //CHIP8EMULATOR_BUZZER_H
