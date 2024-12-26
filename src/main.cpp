#include <array>
#include <bitset>
#include <iostream>
#include <format>

#include "glad/gl.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "SDL.h"
#include "SDL_video.h"

#include "gl_helpers.hpp"
#include "shader_linker.hpp"

const unsigned int MASK_X = 0x0003FF;
const unsigned int MASK_Y = 0x0FFC00;
const unsigned int MASK_S = 0x100000;
const unsigned int MASK_T = 0x200000;
const unsigned int MASK_NONE = 0xFFC00000;
static unsigned int packVertex(unsigned int x, unsigned int y, unsigned int s, unsigned int t)
{
    unsigned int vertex = 0;
    vertex |= (x <<  0) & MASK_X;
    vertex |= (y << 10) & MASK_Y;
    vertex |= (s << 20) & MASK_S;
    vertex |= (t << 21) & MASK_T;

    std::cout << std::bitset<32>(vertex) << std::endl;
    float d_x = float((vertex&MASK_X) >>  0);
    float d_y = float((vertex&MASK_Y) >> 10);
    float d_s = float((vertex&MASK_S) >> 20);
    float d_t = float((vertex&MASK_T) >> 21);
    std::cout << std::format("x:{}, y:{}, s:{}, t:{}", d_x, d_y, d_s, d_t) << std::endl;
    return vertex;
}

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

    std::array<unsigned int, 4> verts = {
        packVertex(0, 1, 0, 1),
        packVertex(1, 1, 1, 1),
        packVertex(1, 0, 1, 0),
        packVertex(0, 0, 0, 0),
    };
    const unsigned int numIndices = 6;
    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };
    float worldProjMat[] = {
        2.0f,  0.0f, -1.0f,
        0.0f, -2.0f,  1.0f,
        0.0f,  0.0f,  0.0f
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(unsigned int), &verts[0], GL_STATIC_DRAW);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, 0);
    glEnableVertexAttribArray(0);

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
    std::cout << pairs[0].fragPath << " | " << pairs[0].vertPath << std::endl;
    glUseProgram(shader);
    int uRes = glGetUniformLocation(shader, "u_Res");
    int uWorldProj = glGetUniformLocation(shader, "u_WorldProj");
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

        // Update uniforms
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glUniform2f(uRes, (float)w, (float)h);

        // Clear main screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Render quad
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
