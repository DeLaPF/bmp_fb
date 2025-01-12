#pragma once

#include "SDL.h"

#include "application.hpp"
#include "sdl_gl.hpp"

namespace BFGe {
    class Engine {
    public:
        Engine();
        ~Engine();

        void RunApp(BFGe::Application* app);

        SDL_Window* getWindow() { return this->m_window; }
        SDLEventHandler* getEventHandler() { return this->m_eventHandler; }
    private:
        // TODO: breakout into WindowManagerClass
        SDL_Window* m_window;
        SDL_GLContext m_sdlGLContext;
        SDLEventHandler* m_eventHandler;
    };
}
