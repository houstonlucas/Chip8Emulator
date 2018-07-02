cmake_minimum_required(VERSION 3.8)
project(Chip8Emulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp CPU.cpp CPU.h Instruction.cpp Instruction.h Screen.cpp Screen.h Chip8Emulator.cpp Chip8Emulator.h)
add_executable(Chip8Emulator ${SOURCE_FILES} CPU.cpp CPU.h Instruction.cpp Instruction.h Screen.cpp Screen.h Chip8Emulator.cpp Chip8Emulator.h)



TARGET_LINK_LIBRARIES(
        Chip8Emulator
        GL
        GLEW
        SDL2
)