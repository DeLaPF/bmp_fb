#include "glad/gl.h"
#include "imgui_impl_sdl2.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

void app(SDL_Window* window)
{
    // TODO: update to bitmapDim and renderDim
    unsigned int bmpW = 11;
    unsigned int bmpH = 11;
    int fbW = 400;
    int fbH = 400;
    BitmapFramebuffer bfb(bmpW, bmpH, fbW, fbH);

    // Draw pattern on bitmap
    auto bitmap = bfb.getBitmap();
    for (auto i = 0; i < bitmap->size(); i++) { bitmap->at(i) = i%2 == 0 ? 0x1F7AC4 : 0; }
    bfb.updateBitmap();

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
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    // TODO: why is this resizing bfb's viewport?
                    glViewport(0, 0, w, h);
                }
            }
        }
        newImguiFrame();

        // Clear main screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: how to provide default dimensions (and style)?
        ImGui::Begin("Display");
        int newW = (int)ImGui::GetContentRegionAvail().x;
        int newH = (int)ImGui::GetContentRegionAvail().y;
        if (newW != fbW || newH != fbH) {
            bfb.resizeFb(newW, newH);
            fbW = newW;
            fbH = newH;
        }

        // render bitmap fb
        bfb.render();

        ImVec2 guiPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            bfb.getTextureId(),
            ImVec2(guiPos.x, guiPos.y),
            ImVec2(guiPos.x + fbW, guiPos.y + fbH),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::End();

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
