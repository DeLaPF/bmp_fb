#pragma once

#include "SDL.h"

#include "application.hpp"
#include "sdl_gl.hpp"

namespace BFGe {
    class Engine {
    public:
        Engine(int argc = 0, char** argv = nullptr);
        ~Engine();

        void RunApp(BFGe::Application* app);

        SDL_Window* getWindow() { return this->m_window; }
        SDLEventHandler* getEventHandler() { return this->m_eventHandler; }

        int getArgC() { return this->m_argc; }
        char** getArgV() { return this->m_argv; }
    private:
        // TODO: maybe handle differently
        int m_argc;
        char** m_argv;

        // TODO: breakout into WindowManagerClass
        SDL_Window* m_window;
        SDL_GLContext m_sdlGLContext;
        SDLEventHandler* m_eventHandler;
    };
}
