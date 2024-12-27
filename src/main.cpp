#include <array>
#include <iostream>
#include <format>

#include "glad/gl.h"
#include "imgui_impl_sdl2.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "resizable_target.hpp"
#include "sdl_gl.hpp"
#include "shader_linker.hpp"

std::array<float, 9> modelViewMatrix(int w, int h, int mW, int mH)
{
    float aspRW = w >= h ? ((float)w)/h : 1.0f;
    float aspRH = w >= h ? 1.0f : ((float)h)/w;
    float transW = -1.0f + (w >= h ? (w-h)/((float)w) : 0.0f);
    float transH =  1.0f + (w >= h ? 0.0f : (w-h)/((float)h));
    return {
        2.0f/(mW*aspRW), 0.0f, transW,
        0.0f, -2.0f/(mH*aspRH), transH,
        0.0f, 0.0f, 0.0f
    };
}

int main(int argc, char **argv) {
    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE).value();
    SDL_GLContext glContext = initSDLGLContext(window).value();
    initImguiGL(window, glContext);

    // TODO: in essence all of this only renders a bimap to an imgui window
    // how can I best decrease the current verbosity?
    int w = 400;
    int h = 400;
    ResizeableTarget imguiTarget(w, h);

    unsigned int bmpW = 11;
    unsigned int bmpH = 11;
    BitmapFramebuffer bfb(bmpW, bmpH);
    auto bitmap = bfb.getBitmap();
    for (auto i = 0; i < bitmap->size(); i++) { bitmap->at(i) = i%2 == 0 ? 0x1F7AC4 : 0; }
    bfb.updateBitmap();
    std::array<float, 9> mvm = modelViewMatrix(w, h, bmpW, bmpH);

    const char* relPath = "res/shaders";
    std::vector<ShaderPair> pairs = linkShaders(relPath);
    std::vector<unsigned int> shaderIds = compileShaders(pairs);
    if (shaderIds.size() < 1) {
        std::cout << std::format(
            "ERROR [Shaders]: no valid shaders found at: {}",
            relPath
        ) << std::endl;
        exit(1);
    }
    unsigned int shader = shaderIds[0];
    glUseProgram(shader);

    int uRes = glGetUniformLocation(shader, "u_Res");
    int uBitmapDim = glGetUniformLocation(shader, "u_BitmapDim");
    int uMVM = glGetUniformLocation(shader, "u_ModelViewMat");
    glUniform2i(uRes, w, h);
    glUniform2i(uBitmapDim, bmpW, bmpH);
    glUniformMatrix3fv(uMVM, 1, GL_TRUE, &mvm[0]);

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
                    glViewport(0, 0, w, h);
                }
            }
        }
        newImguiFrame();

        // Clear main screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: how to provide default dimensions (and style)?
        imguiTarget.bind();
        ImGui::Begin("Display");
        int newW = (int)ImGui::GetContentRegionAvail().x;
        int newH = (int)ImGui::GetContentRegionAvail().y;
        if (newW != w || newH != h) {
            imguiTarget.resize(newW, newH);
            w = newW;
            h = newH;
            mvm = modelViewMatrix(w, h, bmpW, bmpH);
            glUniformMatrix3fv(uMVM, 1, GL_TRUE, &mvm[0]);
            glViewport(0, 0, w, h);
        }

        // render bitmap fb
        bfb.render();

        ImVec2 guiPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            imguiTarget.getTextureId(),
            ImVec2(guiPos.x, guiPos.y),
            ImVec2(guiPos.x + w, guiPos.y + h),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::End();
        imguiTarget.unbind();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);
    }

    // shader cleanup
    for (int i = 0; i < shaderIds.size(); i++) {
        if (shaderIds[i] != -1) {
            glDeleteProgram(shaderIds[i]);
        }
    }

    destroyImguiGL();
    destroySDLGL(window, glContext);

    return 0;
}
