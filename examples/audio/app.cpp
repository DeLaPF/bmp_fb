#include "app.hpp"

#include "sdl_gl.hpp"

Audio::Application::Application() {}
Audio::Application::~Application() {}

void Audio::Application::onInit()
{
    wavM.playWave(4, 10000);
}

void Audio::Application::onTick()
{
}

// Defines the application that will be ran by the engine
BFGengine::Application* BFGengine::CreateApplication()
{
    initSDLSubsystem(SDL_INIT_AUDIO);

    return new Audio::Application();
}
