//
// Created by namesnonames on 7/7/18.
//

#include "Buzzer.h"



void audio_callback(void* user_data, uint8_t * raw_buffer, int bytes){
    short * buffer = (short*)raw_buffer;
    int length = bytes/2;
    int &sample_nr(*(int*)user_data);

    for(int i = 0; i < length; i++, sample_nr++){
        double time = (double)sample_nr/(double)SAMPLE_RATE;
        buffer[i] = (short)(AMPLITUDE* sin(2.0f*M_PI*441.0f*time));
    }
}

Buzzer::Buzzer() {
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    }


    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 2048;
    want.callback = audio_callback;
    want.userdata = &sample_nr;

    if(SDL_OpenAudio(&want, &have) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
    }
    if(want.format != have.format){
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");
    }

}

void Buzzer::stopBeep() {
    isPlaying = false;
    SDL_PauseAudio(1);
}

void Buzzer::startBeep() {
    isPlaying = true;
    SDL_PauseAudio(0);
}

Buzzer::~Buzzer() {
    SDL_CloseAudio();
}
