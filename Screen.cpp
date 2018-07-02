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

void Screen::updateScreen(uint8_t *vram) {
    float cellWidth = width/float(numCellsWide);
    float cellHeight = height/float(numCellsTall);
    float vertices[] = {
            0.0f,  cellHeight,
            cellWidth,  cellHeight,
            cellWidth, 0.0f,
            0.0f, 0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    for(int x = 0; x < numCellsWide; x++){
        for(int y = 0; y < numCellsTall; y++){
            glPushMatrix();
            float c = (x+y)%2;
            glColor3f(c, c, c);
            glTranslatef(x*cellWidth, y*cellHeight, 0.0f);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_QUADS, 0, 4);
            glPopMatrix();
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    SDL_GL_SwapWindow(window);
}
