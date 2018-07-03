//
// Created by namesnonames on 7/2/18.
//

#include "Chip8Emulator.h"

Chip8Emulator::Chip8Emulator(){
    memory = new uint8_t[4096];
    initMainWindow(windowTitle);
    screen = new Screen(window);
}

void Chip8Emulator::runEmulation()
{
    uint8_t bytes[] = {
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xfe, 0xf7, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xfe, 0x17, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xfe, 0xf7, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xfe, 0x87, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xfe, 0xf7, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
            0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
            0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
    };



    bool emulation_is_running = true;
    while(emulation_is_running){
        SDL_Event event{};

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                emulation_is_running = false;
            }

        }

        screen->updateScreen(bytes);
        SDL_Delay((1 / 60.0 * 1000));
    }
}

Chip8Emulator::~Chip8Emulator()
{
    cleanupSDL();
    delete screen;
}

void Chip8Emulator::loadROM(std::string romPath) {
    std::ifstream fin(romPath);

    int address = 0x200;
    uint8_t byte;
    while(fin.good()){
        byte = (uint8_t)fin.get();
        std::cout << (int)byte << std::endl;
        memory[address] = byte;
        address++;
    }
    std::cout << address-0x200 << std::endl;

    fin.close();
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
