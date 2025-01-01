#include "imgui_gl.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

void initImguiGL(SDL_Window* window, SDL_GLContext glContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void newImguiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void renderImguiFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void destroyImguiGL()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

TextureWindow::TextureWindow(std::string name, unsigned int width, unsigned int height)
{
    this->m_name = name;
    this->m_width = width;
    this->m_height = height;
    this->m_didResize = false;
}

void TextureWindow::drawWindow(int textureId)
{
    ImGui::Begin(this->m_name.c_str(), NULL, ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowSize(ImVec2((float)this->m_width, (float)this->m_height), ImGuiCond_FirstUseEver);
    ImVec2 guiPos = ImGui::GetCursorScreenPos();
    ImVec2 guiDim = ImGui::GetContentRegionAvail();
    this->m_didResize = (int)guiDim.x != this->m_width || (int)guiDim.y != this->m_height;
    if (this->m_didResize) {
        this->m_width = (int)guiDim.x;
        this->m_height = (int)guiDim.y;
    }

    ImGui::GetWindowDrawList()->AddImage(
        textureId,
        ImVec2(guiPos.x, guiPos.y),
        ImVec2(guiPos.x + guiDim.x, guiPos.y + guiDim.y),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
    ImGui::End();
}

bool TextureWindow::didResize() { return this->m_didResize; }
unsigned int TextureWindow::width() { return this->m_width; }
unsigned int TextureWindow::height() { return this->m_height; }
