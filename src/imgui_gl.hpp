#pragma once

#include <string>

#include "SDL.h"

void initImguiGL(SDL_Window* window, SDL_GLContext glContext);
void newImguiFrame();
void renderImguiFrame();
void destroyImguiGL();

class TextureWindow {
public:
    TextureWindow(std::string name, int textureId, unsigned int width = 400, unsigned int height = 400);

    void drawWindow();

    bool didResize();
    unsigned int width();
    unsigned int height();
private:
    int m_textureId;
    std::string m_name;
    unsigned int m_width;
    unsigned int m_height;
    bool m_didResize;
};
