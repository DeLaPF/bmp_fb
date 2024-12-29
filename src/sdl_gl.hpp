#pragma once

#include <optional>
#include <string>

#include "SDL.h"

std::optional<SDL_Window*> initSDLGLWindow(int w, int h, int flags, std::string name = "SDL Window");
std::optional<SDL_GLContext> initSDLGLContext(SDL_Window* window);
void destroySDLGL(SDL_Window* window, SDL_GLContext glContext);

// TODO: extend when/where needed
// TODO: add keymap
class SDLEventHandler {
public:
    SDLEventHandler(SDL_Window* window);

    void handleEvents();

    bool windowShouldClose() { return this->m_windowShouldClose; }
    int windowW() { return this->m_windowW; }
    int windowH() { return this->m_windowH; }
private:
    SDL_Window* m_window;
    int m_windowW;
    int m_windowH;
    bool m_windowShouldClose;
};
