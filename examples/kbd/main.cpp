#include <chrono>
#include <iostream>
#include <thread>

#include "glad/gl.h"
#include "SDL.h"

#include "BFGe/bitmap_fb.hpp"
#include "BFGe/imgui_gl.hpp"
#include "BFGe/sdl_gl.hpp"

struct iVec2 {
    int x, y;
};

void app(SDL_Window* window)
{
    SDLEventHandler eH(window);
    BitmapFramebuffer bfb0(101, 101, 400, 400);
    TextureWindow bfbDisp0("Display 0");

    // Draw pattern on bitmap(s)
    auto bitmap0 = bfb0.getBitmap();

    // "Player" pos
    iVec2 cPos = { 0, 0 };
    iVec2 pPos = { 0, 0 };

    // Main loop
    while (!eH.windowShouldClose()) {
        unsigned int frameStart = SDL_GetTicks();
        eH.handleEvents();
        newImguiFrame();

        if (eH.isANKeyPressed(SDL_SCANCODE_W)) {
            cPos.y = (cPos.y-1 + bfb0.bmpH())%bfb0.bmpH();
        }
        if (eH.isANKeyPressed(SDL_SCANCODE_S)) {
            cPos.y = (cPos.y+1 + bfb0.bmpH())%bfb0.bmpH();
        }
        if (eH.isANKeyPressed(SDL_SCANCODE_A)) {
            cPos.x = (cPos.x-1 + bfb0.bmpW())%bfb0.bmpW();
        }
        if (eH.isANKeyPressed(SDL_SCANCODE_D)) {
            cPos.x = (cPos.x+1 + bfb0.bmpW())%bfb0.bmpW();
        }
        if (cPos.x != pPos.x || cPos.y != pPos.y) {
            int pInd = pPos.y*bfb0.bmpW() + pPos.x;
            int cInd = cPos.y*bfb0.bmpW() + cPos.x;
            bitmap0->at(pInd) = 0x000000FF;
            bitmap0->at(cInd) = 0xFFFFFFFF;
            bfb0.updateBitmap();
            pPos.x = cPos.x;
            pPos.y = cPos.y;
        }

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bfbDisp0.drawWindow(bfb0.getTextureId());
        if (bfbDisp0.didResize()) { bfb0.resizeRenderDim(bfbDisp0.width(), bfbDisp0.height()); }
        bfb0.render();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        int waitTime = (1000/60) - frameTime;
        if (waitTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
        } else if (waitTime < 0) {
            std::cout << "running slow!!! (" << frameTime << "ms per frame)" << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    if (initSDL(SDL_INIT_VIDEO, SDL_GL_CONTEXT_DEBUG_FLAG) < 0) { return 1; }
    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext glContext = initSDLGLContext(window).value();
    initImguiGL(window, glContext);

    // ensures contents of app are destroyed before glContext
    app(window);

    destroyImguiGL();
    destroySDLGL(window, glContext);

    return 0;
}
