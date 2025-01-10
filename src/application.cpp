#include "application.hpp"

#include "glad/gl.h"
#include "SDL.h"

#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

static void loop(BFGengine::Application* app, SDL_Window* window)
{
    SDLEventHandler eH(window);
    while (!eH.windowShouldClose()) {
        eH.handleEvents();
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        app->onTick();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);
    }
}

namespace BFGengine {
    Application::~Application() {}

    void Application::Run()
    {
        if (initSDL(SDL_INIT_VIDEO, SDL_GL_CONTEXT_DEBUG_FLAG) < 0) { return; }
        SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDL_GLContext glContext = initSDLGLContext(window).value();
        initImguiGL(window, glContext);

        this->onInit();

        // ensures contents of app are destroyed before glContext
        loop(this, window);

        destroyImguiGL();
        destroySDLGL(window, glContext);
    }
}
