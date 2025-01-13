#pragma once

#include "BFGe/BFGe.hpp"
#include "BFGe/application.hpp"
#include "BFGe/audio.hpp"

namespace Audio {
    class Application : public BFGe::Application {
    public:
        Application(BFGe::Engine*);
        virtual ~Application() override;

        virtual void onInit() override;
        virtual void onTick() override;

    private:
        WaveManager m_wavM;
        BFGe::Engine* m_engine;
    };
}
