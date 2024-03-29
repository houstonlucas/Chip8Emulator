//
// Created by namesnonames on 7/2/18.
//

#include "Chip8Emulator.h"

Chip8Emulator::Chip8Emulator(){
    memory = new uint8_t[4096];
    initMainWindow(windowTitle);
    screen = new Screen(window);
    cpu = new CPU();

    // Initialize keys
    for(bool &b: inputValues){
        b = false;
    }

    initializeSpriteMemory();
    initializeKeyMapping();
}

void Chip8Emulator::runEmulation()
{

    bool emulation_is_running = true;
    while(emulation_is_running){
        SDL_Event event{};

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                emulation_is_running = false;
            }
            updateInputValues(event);
            if (event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    emulation_is_running = false;
                }
            }
        }

        cpu->processInstruction(this);

        //TODO: call this at 60Hz
        cpu->handleSound();
    }
}

Chip8Emulator::~Chip8Emulator()
{
    cleanupSDL();
    delete screen;
    delete memory;
}

bool Chip8Emulator::loadROM(std::string romPath) {
    std::ifstream fin(romPath);

    if(!fin.good()){
        fin.close();
        return false;
    }

    uint16_t address = PROGRAM_COUNTER_START;
    uint8_t byte;
    while(fin.good()){
        byte = (uint8_t)fin.get();
        memory[address] = byte;
        address++;
    }

    return true;
}

//
// The following functions:
// (
//      initMainWindow,
//      checkSDLError,
//      setOpenGLAttributes,
//      cleanupSDL
// )
// were heavily 'inspired' by http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
//    ds
bool Chip8Emulator::initMainWindow(const std::string &title)
{
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return false;
    }

    // Create our window centered
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    // Check that everything worked out okay
    if (!window)
    {
        std::cout << "Unable to create window\n";
        checkSDLError(__LINE__);
        return false;
    }

    // Create our opengl context and attach it to our window
    context = SDL_GL_CreateContext(window);

    setOpenGLAttributes();

    // This makes our buffer swap synchronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    return true;
}

bool Chip8Emulator::setOpenGLAttributes()
{
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL, but most video cards would be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}

void Chip8Emulator::checkSDLError(int line = -1)
{
    std::string error = SDL_GetError();
    if (!error.empty())
    {
        std::cout << "SLD Error : " << error << std::endl;
        if (line != -1) {
            std::cout << "\nLine : " << line << std::endl;
        }
        SDL_ClearError();
    }
}

void Chip8Emulator::cleanupSDL()
{
    // Delete our OpengL context
    SDL_GL_DeleteContext(context);
    // Destroy our window
    SDL_DestroyWindow(window);
    // Shutdown SDL 2
    SDL_Quit();
}

void Chip8Emulator::initializeSpriteMemory() {
    uint8_t sprite0[] = {
            0xf0,
            0x90,
            0x90,
            0x90,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0], sprite0, 5);

    uint8_t sprite1[] = {
            0x20,
            0x60,
            0x20,
            0x20,
            0x70,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[1], sprite1, 5);
    uint8_t sprite2[] = {
            0xf0,
            0x10,
            0xf0,
            0x80,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[2], sprite2, 5);
    uint8_t sprite3[] = {
            0xf0,
            0x10,
            0xf0,
            0x10,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[3], sprite3, 5);
    uint8_t sprite4[] = {
            0x90,
            0x90,
            0xf0,
            0x10,
            0x10,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[4], sprite4, 5);
    uint8_t sprite5[] = {
            0xf0,
            0x80,
            0xf0,
            0x10,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[5], sprite5, 5);
    uint8_t sprite6[] = {
            0xf0,
            0x80,
            0xf0,
            0x90,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[6], sprite6, 5);
    uint8_t sprite7[] = {
            0xf0,
            0x10,
            0x20,
            0x40,
            0x40,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[7], sprite7, 5);
    uint8_t sprite8[] = {
            0xf0,
            0x90,
            0xf0,
            0x90,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[8], sprite8, 5);
    uint8_t sprite9[] = {
            0xf0,
            0x90,
            0xf0,
            0x10,
            0x10,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[9], sprite9, 5);
    uint8_t spriteA[] = {
            0xf0,
            0x90,
            0xf0,
            0x90,
            0x90,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xa], spriteA, 5);
    uint8_t spriteB[] = {
            0xe0,
            0x90,
            0xe0,
            0x90,
            0xe0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xb], spriteB, 5);
    uint8_t spriteC[] = {
            0xf0,
            0x80,
            0x80,
            0x80,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xc], spriteC, 5);
    uint8_t spriteD[] = {
            0xe0,
            0x90,
            0x90,
            0x90,
            0xe0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xd], spriteD, 5);
    uint8_t spriteE[] = {
            0xf0,
            0x80,
            0xf0,
            0x80,
            0xf0,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xe], spriteE, 5);
    uint8_t spriteF[] = {
            0xf0,
            0x80,
            0xf0,
            0x80,
            0x80,
    };
    memcpy(memory+cpu->SPRITE_ADDRESSES[0xf], spriteF, 5);
}

bool Chip8Emulator::updateInputValues(SDL_Event &event) {
    bool somethingChanged = false;
    if(event.type == SDL_KEYDOWN | event.type == SDL_KEYUP) {
        bool pressed = event.type == SDL_KEYDOWN;
        auto it = keyMapping.find(event.key.keysym.sym);
        if(it != keyMapping.end()){
            uint8_t index = keyMapping[event.key.keysym.sym];
            inputValues[index] = pressed;
            somethingChanged = true;
        }
    }
    return somethingChanged;
}

void Chip8Emulator::initializeKeyMapping() {
    //TODO: Load config file?
    keyMapping[SDLK_1] = 0x01;
    keyMapping[SDLK_2] = 0x02;
    keyMapping[SDLK_3] = 0x03;
    keyMapping[SDLK_4] = 0x0c;
    keyMapping[SDLK_q] = 0x04;
    keyMapping[SDLK_w] = 0x05;
    keyMapping[SDLK_e] = 0x06;
    keyMapping[SDLK_r] = 0x0d;
    keyMapping[SDLK_a] = 0x07;
    keyMapping[SDLK_s] = 0x08;
    keyMapping[SDLK_d] = 0x09;
    keyMapping[SDLK_f] = 0x0e;
    keyMapping[SDLK_z] = 0x0a;
    keyMapping[SDLK_x] = 0x00;
    keyMapping[SDLK_c] = 0x0b;
    keyMapping[SDLK_v] = 0x0f;
}
