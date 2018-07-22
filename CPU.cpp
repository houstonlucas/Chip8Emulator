//
// Created by namesnonames on 7/1/18.
//

#include "CPU.h"

void CPU::processInstruction(Chip8Emulator *emulator) {
    uint8_t *memory = emulator->memory;
    Screen *screen = emulator->screen;
    const bool *inputValues = emulator->inputValues;


    uint8_t leftByte = memory[programCounter];
    uint8_t rightByte = memory[programCounter+1];

    uint16_t highAddress =(uint16_t)((leftByte & rightNibbleMask) << 8) ;
    uint16_t address = (highAddress) ^ rightByte;

    uint8_t x = leftByte & rightNibbleMask;
    uint8_t y = (rightByte & leftNibbleMask) >> 4;
    uint8_t k = rightByte & rightNibbleMask;

    programCounter += 2;

    switch((leftByte & leftNibbleMask) >> 4){
        case 0x0:
            // CLS and RET
            if(rightByte == 0xE0){
                screen->clearScreen();
            }else if(rightByte == 0xEE){
                // programCounter = memory[stackPointer];
                memcpy(&programCounter, memory + stackPointer, 2);
                stackPointer -= 2;
            }else{
                //SYS address???
            }
            break;
        case 0x1:
            // JP address
            programCounter = address;
            break;
        case 0x2:
            // CALL address
            stackPointer += 2;
            // memory[stackPointer] = programCounter;
            memcpy(memory + stackPointer, &programCounter, 2);
            programCounter = address;
            break;
        case 0x3:
            // SE Vx kk
            if(V[x] == rightByte){
                programCounter += 2;
            }
            break;
        case 0x4:
            // SNE Vx kk
            if(V[x] != rightByte){
                programCounter += 2;
            }
            break;
        case 0x5:
            // SE Vx Vy 0
            if(V[x] == V[y]){
                programCounter += 2;
            }
            break;
        case 0x6:
            // LD Vx kk
            V[x] = rightByte;
            break;
        case 0x7:
            // Add Vx kk
            V[x] += rightByte;
            break;
        case 0x8:
            // Various Math
            process0x8(x, y, k);
            break;
        case 0x9:
            // SNE Vx Vy 0
            if(V[x] != V[y]){
                programCounter += 2;
            }
            break;
        case 0xa:
            // LD I address
            I = address;
            break;
        case 0xb:
            // JP V0 address
            programCounter = V[0] + address;
            break;
        case 0xc:
            // RND Vx kk
            V[x] = ((uint8_t) random()) & rightByte;
            break;
        case 0xd:
            // DRW Vx Vy k
            screen->drawSprite(memory, I, k, V[x], V[y]);
            break;
        case 0xe:
            // Skip instructions
            if(rightByte == 0x9e){
                if(inputValues[V[x]]){
                    programCounter += 2;
                }
            }else if(rightByte == 0xa1){
                if(!inputValues[V[x]]){
                    programCounter += 2;
                }
            }
            break;
        case 0xf:
            // Various special operations
            process0xF(emulator, x, rightByte);
            break;
        default:
            //NOP I guess
            // ¯\_(ツ)_/¯
            break;
    }
}

uint16_t CPU::getProgramCounter() const {
    return programCounter;
}

void CPU::process0x8(uint8_t x, uint8_t y, uint8_t k) {
    uint16_t result;
    switch(k){
        case 0: // LD
            V[x] = V[y];
            break;
        case 1: // OR
            V[x] = V[x] | V[y];
            break;
        case 2: // AND
            V[x] = V[x] & V[y];
            break;
        case 3: // XOR
            V[x] = V[x] ^ V[y];
            break;
        case 4: // ADD
            result = V[x] + V[y];
            if(result > 255){
                V[0xf] = 1;
            }else{
                V[0xf] = 0;
            }
            V[x] = static_cast<uint8_t>(result);
            break;
        case 5: // SUB
            V[x] = V[x] - V[y];
            if(V[x] > V[y]){
                V[0xf] = 1;
            }else{
                V[0xf] = 0;
            }
            break;
        case 6: // SHR
            // Set flag
            V[0xf] = V[x] & (uint8_t)0x01;
            V[x] = V[x] >> 1;
            break;
        case 7: // SUBN
            if(V[y] > V[x]){
                V[0xf] = 1;
            }else{
                V[0xf] = 0;
            }
            V[x] = V[y] - V[x];
            break;
        case 0xe: // SHL
            // Set flag
            V[0xf] = (V[x] & (uint8_t)0x80) >> (uint8_t)0x7;
            V[x] = V[x] << 1;
            break;
        default:
            break;
    }
}

void CPU::process0xF(Chip8Emulator *emulator, uint8_t x, uint8_t rightByte) {
    uint8_t *memory = emulator->memory;
    uint8_t hundreds;
    uint8_t tens;
    uint8_t val = V[x];
    switch(rightByte){
        case 0x07:
            V[x] = delayTimer;
            break;
        case 0x0a:
            waitForKey(emulator, x);
            break;
        case 0x15:
            delayTimer = V[x];
            break;
        case 0x18:
            soundTimer = V[x];
            break;
        case 0x1e:
            I += V[x];
            break;
        case 0x29:
            loadSpriteAddress(x);
            break;
        case 0x33:
            // BCD encoding into I, I+1, I+2 addresses
            hundreds = val/(uint8_t)100;
            memory[I] = hundreds;
            val -= hundreds*100;
            tens = val/(uint8_t)10;
            memory[I + 1] = tens;
            val -= tens*10;
            memory[I + 2] = val;
            break;
        case 0x55:
            for(int i = 0; i <= x; i++){
                memory[I+i] = V[i];
            }
            break;
        case 0x65:
            for(int i = 0; i <= x; i++){
                V[i] = memory[I+i];
            }
            break;
        default:
            break;
    }
}

void CPU::loadSpriteAddress(uint8_t x){
    I = (uint16_t) SPRITE_ADDRESSES[V[x]];
}

void CPU::decrementTimers() {
    if(delayTimer != 0){
        delayTimer--;
    }
    if(soundTimer != 0){
        soundTimer--;
    }
}

uint8_t CPU::getStackPointer() const {
    return stackPointer;
}

void CPU::handleSound() {
    if(soundTimer != 0){
        if(!buzzer.isPlaying) {
            buzzer.startBeep();
        }
    }else{
        buzzer.stopBeep();
    }
}

void CPU::waitForKey(Chip8Emulator *emu, uint8_t x) {
    bool keyPressed = false;
    bool oldValues[16];
    memset(oldValues, false, 16);
    std::cout << "Waiting for key press." << std::endl;
    while(!keyPressed){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            memcpy(oldValues,emu->inputValues, 16);
            keyPressed = emu->updateInputValues(event);
            if(keyPressed){
                for(uint8_t i=0; i<16; i++){
                    if(oldValues[i] != emu->inputValues[i]){
                        V[x] = i;
                    }
                }
            }
        }
    }
}

CPU::CPU() {
    programCounter = PROGRAM_COUNTER_START;
    stackPointer = STACK_POINTER_BASE;

    I = 0;
    delayTimer = 0;
    soundTimer = 0;
    for (uint8_t &reg : V) {
        reg = 0;
    }

    leftNibbleMask = 0xf0;
    rightNibbleMask = 0x0f;

}
