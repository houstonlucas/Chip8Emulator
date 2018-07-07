//
// Created by namesnonames on 7/1/18.
//

#include "Screen.h"


uint8_t createMask(int numBits);

Screen::Screen(SDL_Window *mainWindow) {
    SDL_GetWindowSize(mainWindow, &width, &height);
    this->window = mainWindow;
    this->vram = new uint8_t[width * height];
    clearScreen();


    rowSize = 8;


    glViewport(0.0f, 0.0f, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void Screen::updateScreen() {
    float cellWidth = width / float(numCellsWide);
    float cellHeight = height / float(numCellsTall);
    float c;
    float vertices[] = {
            0.0f, cellHeight,
            cellWidth, cellHeight,
            cellWidth, 0.0f,
            0.0f, 0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    for (int y = 0; y < numCellsTall; y++) {
        int rowOffset = y * rowSize;
        for (int x = 0; x < numCellsWide; x++) {
            int byteOffset = x / 8;
            int byte = vram[rowOffset + byteOffset];
            int bit = (byte >> (7 - x % 8)) & 1;
            c = (float) bit;

            glPushMatrix();
            glColor3f(c, c, c);
            glTranslatef(x * cellWidth, height - (y + 1) * cellHeight, 0.0f);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_QUADS, 0, 4);
            glPopMatrix();
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    SDL_GL_SwapWindow(window);
}

void Screen::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    memset(vram, 0, width * height);

}

void Screen::drawSprite(const uint8_t *memory, int address, int numRows, int xPos, int yPos) {
    int numBitsIntoByte = (xPos % 8);
    int numBytesIntoRow = xPos / 8;
    int rowOffset;
    uint8_t eightBytes = 0xff;

    uint8_t rightMask = createMask(numBitsIntoByte);
    uint8_t leftMask = ~rightMask;
    for (int row = 0; row < numRows; row++) {
        //Draw row
        rowOffset = (row + yPos) * rowSize;
        uint8_t spriteByte = memory[address+row];
        uint8_t leftByte = vram[rowOffset + numBytesIntoRow];
        vram[rowOffset + numBytesIntoRow] = leftByte ^ (spriteByte >> numBitsIntoByte);
        if(numBytesIntoRow >= (numCellsWide/8) - 1){
            rowOffset -= rowSize;
        }
        uint8_t rightByte = vram[rowOffset + numBytesIntoRow + 1];
        vram[rowOffset + numBytesIntoRow+1] = rightByte ^ (eightBytes&(spriteByte << (8-numBitsIntoByte)));
    }
}

uint8_t createMask(int numBits){
    return static_cast<uint8_t>((1 << (8-numBits)) - 1);
}

