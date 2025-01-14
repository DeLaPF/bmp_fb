#include "app.hpp"

#include "BFGe/entrypoint.hpp"
#include "BFGe/logging.hpp"
#include "BFGe/sdl_gl.hpp"

Audio::Application::Application(BFGe::Engine* engine) { this->m_engine = engine; }
Audio::Application::~Application() {}

void Audio::Application::onInit() {}

void Audio::Application::onTick()
{
    SDLEventHandler* eH = this->m_engine->getEventHandler();
    if (eH->isANKeyPressed(SDL_SCANCODE_W)) {
        this->m_wavM.playWave(4, 10000);
    } else {
        this->m_wavM.stopSound();
    }
}

// Defines the application that will be ran by the engine
BFGe::Application* BFGe::CreateApplication(BFGe::Engine* engine)
{
    BFGE_CLIENT_WARN("Initializing Audio could fail");
    initSDLSubsystem(SDL_INIT_AUDIO);

    return new Audio::Application(engine);
}
