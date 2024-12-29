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

        // TODO: how to provide default dimensions (and style)?
        ImGui::Begin("Display");
        ImVec2 guiPos = ImGui::GetCursorScreenPos();
        ImVec2 guiDim = ImGui::GetContentRegionAvail();
        if ((int)guiDim.x != fbW || (int)guiDim.y != fbH) {
            bfb.resizeFb((int)guiDim.x, (int)guiDim.y);
            fbW = (int)guiDim.x;
            fbH = (int)guiDim.y;
        }

        // TODO (determine): does `AddImage` use data at texId immediately
        // or save reference and wait for `render`?
        // if immediate, will draw black frame on resize, due to texture reinit
        ImGui::GetWindowDrawList()->AddImage(
            bfb.getTextureId(),
            ImVec2(guiPos.x, guiPos.y),
            ImVec2(guiPos.x + guiDim.x, guiPos.y + guiDim.y),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::End();

        // render bitmap fb
        // Note: moving this outside of the display will cause resize to be one frame behind
        // which causes some minor flashing
        // unless put after, then the frame (may be, not actually sure yet) one fram behind
        // this depends on if `AddImage` immediately uses the data referenced by textureId (behind)
        // or if it waits for `Imgui::render()` before it pulls the texture data (in sync)
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
