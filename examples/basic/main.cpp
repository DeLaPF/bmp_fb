#include "glad/gl.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

void app(SDL_Window* window)
{
    SDLEventHandler eH(window);
    BitmapFramebuffer bfb0(11, 33, 400, 400);
    TextureWindow bfbDisp0("Display 0");
    BitmapFramebuffer bfb1(33, 11, 400, 400, BitmapFramebuffer::SINGLE_BIT);
    TextureWindow bfbDisp1("Display 1");
    BitmapFramebuffer bfb2(23, 23, 400, 400, BitmapFramebuffer::SINGLE_BIT);
    TextureWindow bfbDisp2("Display 2");

    // Draw pattern on bitmap(s)
    auto bitmap0 = bfb0.getBitmap();
    for (auto i = 0; i < bitmap0->size(); i++) { bitmap0->at(i) = i%2 == 0 ? 0x1F7AC4FF : 0; }
    bfb0.updateBitmap();
    auto bitmap1 = bfb1.getBitmap();
    for (auto i = 0; i < bitmap1->size(); i++) { bitmap1->at(i) = 0xAAAAAAAA; }
    bfb1.updateBitmap();
    auto bitmap2 = bfb2.getBitmap();
    for (auto i = 0; i < bitmap2->size(); i++) { bitmap2->at(i) = 0xAAAAAAAA; }
    bfb2.updateBitmap();

    // Main loop
    while (!eH.windowShouldClose()) {
        eH.handleEvents();
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bfbDisp0.drawWindow(bfb0.getTextureId());
        if (bfbDisp0.didResize()) { bfb0.resizeRenderDim(bfbDisp0.width(), bfbDisp0.height()); }
        bfb0.render();
        bfbDisp1.drawWindow(bfb1.getTextureId());
        if (bfbDisp1.didResize()) { bfb1.resizeRenderDim(bfbDisp1.width(), bfbDisp1.height()); }
        bfb1.render();
        bfbDisp2.drawWindow(bfb2.getTextureId());
        if (bfbDisp2.didResize()) { bfb2.resizeRenderDim(bfbDisp2.width(), bfbDisp2.height()); }
        bfb2.render();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);
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
