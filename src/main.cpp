#include "glad/gl.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

void app(SDL_Window* window)
{
    SDLEventHandler eH(window);
    BitmapFramebuffer bfb0(11, 11, 400, 400);
    TextureWindow bfbDisp0("Display 0", bfb0.getTextureId());
    BitmapFramebuffer bfb1(11, 11, 400, 400);
    TextureWindow bfbDisp1("Display 1", bfb1.getTextureId());

    // Draw pattern on bitmap(s)
    auto bitmap0 = bfb0.getBitmap();
    for (auto i = 0; i < bitmap0->size(); i++) { bitmap0->at(i) = i%2 == 0 ? 0x1F7AC4 : 0; }
    bfb0.updateBitmap();
    auto bitmap1 = bfb1.getBitmap();
    for (auto i = 0; i < bitmap1->size(); i++) { bitmap1->at(i) = i%3 == 0 ? 0x1F7AC4 : 0; }
    bfb1.updateBitmap();

    // Main loop
    while (!eH.windowShouldClose()) {
        eH.handleEvents();
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bfbDisp0.drawWindow();
        if (bfbDisp0.didResize()) { bfb0.resizeRenderDim(bfbDisp0.width(), bfbDisp0.height()); }
        bfb0.render();
        bfbDisp1.drawWindow();
        if (bfbDisp1.didResize()) { bfb1.resizeRenderDim(bfbDisp1.width(), bfbDisp1.height()); }
        bfb1.render();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);
    }
}

int main(int argc, char **argv) {
    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE).value();
    SDL_GLContext glContext = initSDLGLContext(window).value();
    initImguiGL(window, glContext);

    // ensures contents of app are destroyed before glContext
    app(window);

    destroyImguiGL();
    destroySDLGL(window, glContext);

    return 0;
}
