#include "app.hpp"

#include "sdl_gl.hpp"

Audio::Application::Application() :
    bfb(11, 33, 400, 400),
    bfbDisp("Display 0"),
    wavM(5)
{}
Audio::Application::~Application() {}

void Audio::Application::onInit()
{
    // Draw test patterns on bitmap
    auto bitmap = this->bfb.getBitmap();
    for (auto i = 0; i < bitmap->size(); i++) { bitmap->at(i) = i%2 == 0 ? 0x1F7AC4FF : 0; }
    this->bfb.updateBitmap();

    wavM.genWaveSamples();
    wavM.playSample();
}

void Audio::Application::onTick()
{
    // Handle bfg window resizing
    this->bfbDisp.drawWindow(this->bfb.getTextureId());
    if (this->bfbDisp.didResize()) { this->bfb.resizeRenderDim(this->bfbDisp.width(), this->bfbDisp.height()); }
    this->bfb.render();
}

// Defines the application that will be ran by the engine
BFGengine::Application* BFGengine::CreateApplication()
{
    initSDLSubsystem(SDL_INIT_AUDIO);

    return new Audio::Application();
}
