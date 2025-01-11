#pragma once

#include "application.hpp"
#include "audio.hpp"

namespace Audio {
    class Application : public BFGengine::Application {
    public:
        Application();
        virtual ~Application() override;

        virtual void onInit() override;
        virtual void onTick() override;

    private:
        WaveManager wavM;
    };
}
