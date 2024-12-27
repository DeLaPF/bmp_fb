#include <iostream>
#include <format>

#include "bitmap_fb.hpp"
#include "glad/gl.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "SDL.h"
#include "SDL_video.h"

#include "gl_helpers.hpp"
#include "shader_linker.hpp"

int main(int argc, char **argv) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    // Create window with OpenGL context
    SDL_Window* window = SDL_CreateWindow(
        "OpenGL with SDL",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL // | SDL_WINDOW_RESIZABLE
    );
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("OpenGL context creation failed: %s", SDL_GetError());
        return 1;
    }
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    std::cout << std::format(
        "GL {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)
    ) << std::endl;
    glDebugMessageCallback(handleGLError, 0);

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    unsigned int bmpW = 11;
    unsigned int bmpH = 11;
    BitmapFramebuffer bfb(bmpW, bmpH);
    auto bitmap = bfb.getBitmap();
    for (auto i = 0; i < bitmap->size(); i++) { bitmap->at(i) = i%2 == 0 ? 0xFFFFFF : 0; }
    bfb.updateBitmap();

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    float worldProjMat[] = {
        2.0f/bmpW,  0.0f, -1.0f,
        0.0f, -2.0f/bmpH,  1.0f,
        0.0f,  0.0f,  0.0f
    };
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
    int uWorldProj = glGetUniformLocation(shader, "u_WorldProj");
    glUniform2i(uRes, w, h);
    glUniform2i(uBitmapDim, bmpW, bmpH);
    glUniformMatrix3fv(uWorldProj, 1, GL_TRUE, worldProjMat);


    // Main loop
    bool running = true;
    while (running) {
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
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Clear main screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bfb.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Swap buffers to display the rendered image
        SDL_GL_SwapWindow(window);
    }

    // gl cleanup
    for (int i = 0; i < shaderIds.size(); i++) {
        if (shaderIds[i] != -1) {
            glDeleteProgram(shaderIds[i]);
        }
    }

    // ImGui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
