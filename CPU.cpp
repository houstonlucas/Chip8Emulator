//
// Created by namesnonames on 7/1/18.
//

#include "CPU.h"

void CPU::processInstruction(uint8_t *memory, Screen *screen) {
    uint8_t leftByte = memory[programCounter];
    uint8_t rightByte = memory[programCounter+1];

    uint16_t highAddress =(uint16_t)((leftByte & rightNibbleMask) << 8) ;
    uint16_t address = (highAddress) ^ rightByte;

    uint8_t x = leftByte & rightNibbleMask;
    uint8_t y = (rightByte & leftNibbleMask) >> 4;
    uint8_t k = rightByte & rightNibbleMask;

    switch((leftByte & leftNibbleMask) >> 4){
        case 0x0:
            // CLS and RET
            if(rightByte == 0xE0){
                screen->clearScreen();
            }else if(rightByte == 0xEE){
                //TODO: RET
            }else{
                //SYS address???
            }
            std::cout << "0x0" << std::endl;
            break;
        case 0x1:
            // JP address
            programCounter = address;
            std::cout << "0x1" << std::endl;
            break;
        case 0x2:
            // CALL address
            std::cout << "0x2" << std::endl;
            break;
        case 0x3:
            // SE Vx kk
            if(V[x] == rightByte){
                programCounter += 2;
            }
            std::cout << "0x3" << std::endl;
            break;
        case 0x4:
            // SNE Vx kk
            if(V[x] != rightByte){
                programCounter += 2;
            }
            std::cout << "0x4" << std::endl;
            break;
        case 0x5:
            // SE Vx Vy 0
            if(V[x] == V[y]){
                programCounter += 2;
            }
            std::cout << "0x5" << std::endl;
            break;
        case 0x6:
            // LD Vx kk
            V[x] = rightByte;
            std::cout << "0x6" << std::endl;
            break;
        case 0x7:
            // Add Vx kk
            V[x] += rightByte;
            std::cout << "0x7" << std::endl;
            break;
        case 0x8:
            // Various Math
            process0x8(x, y, k);
            std::cout << "0x8" << std::endl;
            break;
        case 0x9:
            // SNE Vx Vy 0
            if(V[x] != V[y]){
                programCounter += 2;
            }
            std::cout << "0x9" << std::endl;
            break;
        case 0xa:
            // LD I address
            I = address;
            std::cout << "0xa" << std::endl;
            break;
        case 0xb:
            // JP V0 address
            programCounter = V[0] + address;
            std::cout << "0xb" << std::endl;
            break;
        case 0xc:
            // RND Vx kk
            V[x] = ((uint8_t) random()) & rightByte;
            std::cout << "0xc" << std::endl;
            break;
        case 0xd:
            // DRW Vx Vy k
            screen->drawSprite(memory, I, k, V[x], V[y]);
            std::cout << "0xd" << std::endl;
            break;
        case 0xe:
            // Skip instructions
            std::cout << "0xe" << std::endl;
            break;
        case 0xf:
            // Various flag operations
            process0xF(memory, x, rightByte);
            std::cout << "0xf" << std::endl;
            break;
        default:
            //NOP I guess
            // ¯\_(ツ)_/¯
            break;
    }
    programCounter += 2;
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

void CPU::process0xF(uint8_t *memory, uint8_t x, uint8_t rightByte) {
    uint8_t hundreds;
    uint8_t tens;
    uint8_t val = V[x];
    switch(rightByte){
        case 0x07:
            V[x] = delayTimer;
            break;
        case 0x0a:
            //TODO: input stuff
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
