#pragma once

#include "SDL.h"

void initImguiGL(SDL_Window* window, SDL_GLContext glContext);
void newImguiFrame();
void renderImguiFrame();
void destroyImguiGL();
