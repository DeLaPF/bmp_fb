#include "application.hpp"

int main(int argc, char** argv) {
    BFGengine::Application* app = BFGengine::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
