#include "gl_helpers.hpp"

#include <format>
#include <iostream>

void handleGLError(
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
