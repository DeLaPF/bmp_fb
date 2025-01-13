#include "entrypoint.hpp"

#include "application.hpp"
#include "BFGe.hpp"

int main(int argc, char** argv) {
    // Engine initialization
    BFGe::Engine* engine = new BFGe::Engine(argc, argv);

    // Application entrypoint (defined by client)
    BFGe::Application* app = BFGe::CreateApplication(engine);
    app->onInit();
    engine->RunApp(app);

    // Engine teardown
    delete app;
    delete engine;

    return 0;
}
