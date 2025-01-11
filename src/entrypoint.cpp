#include "glad/gl.h"
#include "SDL.h"

#include "application.hpp"
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

int main(int argc, char** argv) {
    // Engine initialization
    // TODO: will likely want to give some control of sizing, flags, and window name back to the client
    if (initSDL(SDL_INIT_VIDEO, SDL_GL_CONTEXT_DEBUG_FLAG) < 0) { return -1; }
    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext glContext = initSDLGLContext(window).value();
    initImguiGL(window, glContext);

    // Application entrypoint (defined by client)
    // TODO: pass in window pointer on create or init
    BFGengine::Application* app = BFGengine::CreateApplication();
    app->onInit();
    loop(app, window);

    // Engine teardown
    delete app;
    destroyImguiGL();
    destroySDLGL(window, glContext);

    return 0;
}
