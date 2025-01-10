#include "application.hpp"
#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"

namespace Basic {
    class Application : public BFGengine::Application {
    private:
        BitmapFramebuffer* bfb0;
        TextureWindow* bfbDisp0;
        BitmapFramebuffer* bfb1;
        TextureWindow* bfbDisp1;
        BitmapFramebuffer* bfb2;
        TextureWindow* bfbDisp2;

    public:
        Application() {}

        virtual ~Application() override
        {
            delete bfb0;
            delete bfbDisp0;
            delete bfb1;
            delete bfbDisp1;
            delete bfb2;
            delete bfbDisp2;
        }

        virtual void onInit() override
        {
            bfb0 = new BitmapFramebuffer(11, 33, 400, 400);
            bfbDisp0 = new TextureWindow("Display 0");
            bfb1 = new BitmapFramebuffer(33, 11, 400, 400, BitmapFramebuffer::SINGLE_BIT);
            bfbDisp1 = new TextureWindow("Display 1");
            bfb2 = new BitmapFramebuffer(23, 23, 400, 400, BitmapFramebuffer::SINGLE_BIT);
            bfbDisp2 = new TextureWindow("Display 2");

            // Draw pattern on bitmap(s)
            auto bitmap0 = bfb0->getBitmap();
            for (auto i = 0; i < bitmap0->size(); i++) { bitmap0->at(i) = i%2 == 0 ? 0x1F7AC4FF : 0; }
            bfb0->updateBitmap();
            auto bitmap1 = bfb1->getBitmap();
            for (auto i = 0; i < bitmap1->size(); i++) { bitmap1->at(i) = 0xAAAAAAAA; }
            bfb1->updateBitmap();
            auto bitmap2 = bfb2->getBitmap();
            for (auto i = 0; i < bitmap2->size(); i++) { bitmap2->at(i) = 0xAAAAAAAA; }
            bfb2->updateBitmap();
        }

        virtual void onTick() override
        {
            this->bfbDisp0->drawWindow(this->bfb0->getTextureId());
            if (this->bfbDisp0->didResize()) { this->bfb0->resizeRenderDim(this->bfbDisp0->width(), this->bfbDisp0->height()); }
            this->bfb0->render();
            this->bfbDisp1->drawWindow(this->bfb1->getTextureId());
            if (this->bfbDisp1->didResize()) { this->bfb1->resizeRenderDim(this->bfbDisp1->width(), this->bfbDisp1->height()); }
            this->bfb1->render();
            this->bfbDisp2->drawWindow(bfb2->getTextureId());
            if (this->bfbDisp2->didResize()) { this->bfb2->resizeRenderDim(this->bfbDisp2->width(), this->bfbDisp2->height()); }
            this->bfb2->render();
        }
    };
}

BFGengine::Application* BFGengine::CreateApplication()
{
    return new Basic::Application();
}
