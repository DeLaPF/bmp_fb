#include "app.hpp"

Audio::Application::Application(SDL_Window* window, SDLEventHandler* eH)
{
    this->m_window = window;
    this->m_eH = eH;
}
Audio::Application::~Application() {}

void Audio::Application::onInit() {}

void Audio::Application::onTick()
{
    if (this->m_eH->isANKeyPressed(SDL_SCANCODE_W)) {
        m_wavM.playWave(4, 10000);
    } else {
        m_wavM.stopSound();
    }
}

// Defines the application that will be ran by the engine
BFGengine::Application* BFGengine::CreateApplication(SDL_Window* window, SDLEventHandler* eH)
{
    initSDLSubsystem(SDL_INIT_AUDIO);

    return new Audio::Application(window, eH);
}
