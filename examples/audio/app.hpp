#pragma once

#include "application.hpp"
#include "audio.hpp"
#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"

namespace Audio {
    class Application : public BFGengine::Application {
    public:
        Application();
        virtual ~Application() override;

        virtual void onInit() override;

        virtual void onTick() override;

    private:
        BitmapFramebuffer bfb;
        TextureWindow bfbDisp;
        WaveManager wavM;
    };
}
