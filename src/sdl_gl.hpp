#pragma once

#include <optional>
#include <string>

#include "SDL.h"

std::optional<SDL_Window*> initSDLGLWindow(int w, int h, int flags, std::string name = "SDL Window");
std::optional<SDL_GLContext> initSDLGLContext(SDL_Window* window);
void destroySDLGL(SDL_Window* window, SDL_GLContext glContext);
