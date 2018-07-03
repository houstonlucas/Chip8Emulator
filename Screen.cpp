//
// Created by namesnonames on 7/1/18.
//

#include "Screen.h"




Screen::Screen(SDL_Window *mainWindow) {
    SDL_GetWindowSize(mainWindow, &width, &height);
    this->window = mainWindow;

    glViewport(0.0f, 0.0f, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void Screen::updateScreen(const uint8_t *vram) {
    float cellWidth = width/float(numCellsWide);
    float cellHeight = height/float(numCellsTall);
    int rowSize = 8;
    float c;
    float vertices[] = {
            0.0f,  cellHeight,
            cellWidth,  cellHeight,
            cellWidth, 0.0f,
            0.0f, 0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    for(int y = 0; y < numCellsTall; y++){
        int rowOffset = y*rowSize;
        for(int x = 0; x < numCellsWide; x++){
            int byteOffset = x/8;
            int byte = vram[rowOffset + byteOffset];
            int bit = (byte >> (7 - x%8))& 1;
            c = (float)bit;

            glPushMatrix();
            glColor3f(c,c,c);
            glTranslatef(x*cellWidth, height - (y+1)*cellHeight, 0.0f);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_QUADS, 0, 4);
            glPopMatrix();
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    SDL_GL_SwapWindow(window);
}

