#include "sdl_gl.hpp"

#include <format>
#include <iostream>

#include "glad/gl.h"

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
