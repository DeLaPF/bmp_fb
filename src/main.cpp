#include "glad/gl.h"
#include "imgui_impl_sdl2.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

void app(SDL_Window* window)
{
    BitmapFramebuffer bfb(11, 11, 400, 400);
    TextureWindow bfbDisp("Display", bfb.getTextureId());

    // Draw pattern on bitmap
    auto bitmap = bfb.getBitmap();
    for (auto i = 0; i < bitmap->size(); i++) { bitmap->at(i) = i%2 == 0 ? 0x1F7AC4 : 0; }
    bfb.updateBitmap();

    int wW, wH;
    SDL_GetWindowSize(window, &wW, &wH);

    // Main loop
    bool running = true;
    while (running) {
        // TODO: how to handle outside of main without losing flexibility?
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Forward to Imgui
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT
                || (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(window))) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window, &wW, &wH);
                }
            }
        }
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, wW, wH);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bfbDisp.drawWindow();
        if (bfbDisp.didResize()) { bfb.resizeRenderDim(bfbDisp.width(), bfbDisp.height()); }
        bfb.render();

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
