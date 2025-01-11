#include "app.hpp"

Basic::Application::Application() :
    bfb0(11, 33, 400, 400),
    bfbDisp0("Display 0"),
    bfb1(33, 11, 400, 400, BitmapFramebuffer::SINGLE_BIT),
    bfbDisp1("Display 1"),
    bfb2(23, 23, 400, 400, BitmapFramebuffer::SINGLE_BIT),
    bfbDisp2("Display 2")
{}
Basic::Application::~Application() {}

void Basic::Application::onInit()
{
    // Draw test patterns on bitmap
    auto bitmap0 = this->bfb0.getBitmap();
    for (auto i = 0; i < bitmap0->size(); i++) { bitmap0->at(i) = i%2 == 0 ? 0x1F7AC4FF : 0; }
    this->bfb0.updateBitmap();
    auto bitmap1 = this->bfb1.getBitmap();
    for (auto i = 0; i < bitmap1->size(); i++) { bitmap1->at(i) = 0xAAAAAAAA; }
    this->bfb1.updateBitmap();
    auto bitmap2 = this->bfb2.getBitmap();
    for (auto i = 0; i < bitmap2->size(); i++) { bitmap2->at(i) = 0xAAAAAAAA; }
    this->bfb2.updateBitmap();
}

void Basic::Application::onTick()
{
    // Handle bfg window resizing
    this->bfbDisp0.drawWindow(this->bfb0.getTextureId());
    if (this->bfbDisp0.didResize()) { this->bfb0.resizeRenderDim(this->bfbDisp0.width(), this->bfbDisp0.height()); }
    this->bfb0.render();
    this->bfbDisp1.drawWindow(this->bfb1.getTextureId());
    if (this->bfbDisp1.didResize()) { this->bfb1.resizeRenderDim(this->bfbDisp1.width(), this->bfbDisp1.height()); }
    this->bfb1.render();
    this->bfbDisp2.drawWindow(bfb2.getTextureId());
    if (this->bfbDisp2.didResize()) { this->bfb2.resizeRenderDim(this->bfbDisp2.width(), this->bfbDisp2.height()); }
    this->bfb2.render();
}

// Defines the application that will be ran by the engine
BFGengine::Application* BFGengine::CreateApplication(SDL_Window* window, SDLEventHandler* eH)
{
    return new Basic::Application();
}
