#pragma once

#include <optional>
#include <string>

#include "SDL.h"
#include "SDL_scancode.h"

int initSDL(int initFlags, int contextFlags);
int initSDLSubsystem(int flags);
SDL_Window* initSDLGLWindow(int w, int h, int flags, std::string name = "SDL Window");
std::optional<SDL_GLContext> initSDLGLContext(SDL_Window* window);
void destroySDLGL(SDL_Window* window, SDL_GLContext glContext);

// TODO: extend when/where needed
class SDLEventHandler {
public:
    SDLEventHandler(SDL_Window* window);

    void handleEvents();

    bool windowShouldClose() { return this->m_windowShouldClose; }
    int windowW() { return this->m_windowW; }
    int windowH() { return this->m_windowH; }

    bool isANKeyPressed(SDL_Scancode sc) { return this->m_ANkeymap[sc]; }
private:
    SDL_Window* m_window;
    int m_windowW;
    int m_windowH;
    bool m_windowShouldClose;

    // alphanumeric keymap
    // max AN index should be 40 based on [ref](https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_scancode.h#L90)
    bool m_ANkeymap[40];
};
