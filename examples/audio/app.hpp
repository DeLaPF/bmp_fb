#pragma once

#include "application.hpp"
#include "audio.hpp"

namespace Audio {
    class Application : public BFGengine::Application {
    public:
        Application(SDL_Window* window, SDLEventHandler* eH);
        virtual ~Application() override;

        virtual void onInit() override;
        virtual void onTick() override;

    private:
        WaveManager m_wavM;
        SDL_Window* m_window;
        SDLEventHandler* m_eH;
    };
}
