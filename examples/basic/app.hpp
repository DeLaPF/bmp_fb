#pragma once

#include "BFGe/application.hpp"
#include "BFGe/bitmap_fb.hpp"
#include "BFGe/imgui_gl.hpp"

namespace Basic {
    class Application : public BFGe::Application {
    public:
        Application();
        virtual ~Application() override;

        virtual void onInit() override;

        virtual void onTick() override;

    private:
        BitmapFramebuffer bfb0;
        TextureWindow bfbDisp0;
        BitmapFramebuffer bfb1;
        TextureWindow bfbDisp1;
        BitmapFramebuffer bfb2;
        TextureWindow bfbDisp2;
    };
}
