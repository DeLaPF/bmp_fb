#include "sdl_gl.hpp"

#include <format>
#include <iostream>

#include "glad/gl.h"
#include "imgui_impl_sdl2.h"

static void handleGLError(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
)
{
    std::cout << std::format(
        "[GL ERROR]: \n"
        " - source: {}\n"
        " - type: {}\n"
        " - id: {}\n"
        " - severity: {}\n"
        " - Message: {}\n",
        source, type, id, severity, message
    ) << std::endl;
}

std::optional<SDL_Window*> initSDLGLWindow(int w, int h, int flags, std::string name)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return std::nullopt;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    // Create window with OpenGL context
    SDL_Window* window = SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        flags
    );

    return window;
}

std::optional<SDL_GLContext> initSDLGLContext(SDL_Window* window)
{
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("OpenGL context creation failed: %s", SDL_GetError());
        return std::nullopt;
    }

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    std::cout << std::format(
        "GL {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)
    ) << std::endl;
    glDebugMessageCallback(handleGLError, 0);

    return glContext;
}

void destroySDLGL(SDL_Window* window, SDL_GLContext glContext)
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDLEventHandler::SDLEventHandler(SDL_Window* window)
{
    this->m_window = window;
    SDL_GetWindowSize(this->m_window, &this->m_windowW, &this->m_windowH);
    this->m_windowShouldClose = false;
    // TODO: there must be a better way to do this?
    for (int i = 0; i < 40; i++) { this->m_ANkeymap[i] = false; }
}

void SDLEventHandler::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Forward to Imgui
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            this->m_windowShouldClose = true;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(this->m_window))
            {
                this->m_windowShouldClose = true;
            }
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(this->m_window, &this->m_windowW, &this->m_windowH);
            }
            break;
        case SDL_KEYDOWN:
        {
            // TODO: why does defining this variable require enclosing scope?
            // lsp was stating: `Cannot jump from switch statement to this case label`
            auto sc = event.key.keysym.scancode;
            if (sc < 39) { this->m_ANkeymap[sc] = true; }
            break;
        }
        case SDL_KEYUP:
        {
            auto sc = event.key.keysym.scancode;
            if (sc < 39) { this->m_ANkeymap[sc] = false; }
            break;
        }
        default:
            break;
        }
    }
}
