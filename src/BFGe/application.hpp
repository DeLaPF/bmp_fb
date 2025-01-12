#pragma once

namespace BFGe {
    class Application {
    public:
        virtual ~Application() = 0;

        virtual void onInit() = 0;
        virtual void onTick() = 0;
    };
}
