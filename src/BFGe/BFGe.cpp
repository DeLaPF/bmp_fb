#include "BFGe.hpp"

#include <stdexcept>

#include "glad/gl.h"

#include "imgui_gl.hpp"

namespace BFGe {
    Engine::Engine(int argc, char** argv)
    {
        // TODO: will likely want to give some control of sizing, flags, and window name back to the client
        if (initSDL(SDL_INIT_VIDEO, SDL_GL_CONTEXT_DEBUG_FLAG) < 0) {
            std::runtime_error("Unable to initialize SDL");
        }
        this->m_window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        this->m_sdlGLContext = initSDLGLContext(this->m_window).value();
        initImguiGL(this->m_window, this->m_sdlGLContext);

        this->m_eventHandler = new SDLEventHandler(this->m_window);

        this->m_argc = argc;
        this->m_argv = argv;
    }

    Engine::~Engine()
    {
        destroyImguiGL();
        destroySDLGL(this->m_window, this->m_sdlGLContext);
        delete this->m_eventHandler;
    }

    void Engine::RunApp(BFGe::Application* app)
    {
        while (!this->m_eventHandler->windowShouldClose()) {
            this->m_eventHandler->handleEvents();
            newImguiFrame();

            // Clear main screen
            glViewport(0, 0, this->m_eventHandler->windowW(), this->m_eventHandler->windowH());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            app->onTick();

            renderImguiFrame();
            SDL_GL_SwapWindow(this->m_window);
        }
    }
}
