#include "application.hpp"
#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"

namespace Basic {
    class Application : public BFGengine::Application {
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
