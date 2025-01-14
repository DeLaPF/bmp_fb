#include "entrypoint.hpp"

#include "logger.hpp"
#include "application.hpp"
#include "BFGe.hpp"

int main(int argc, char** argv) {
    BFGe::Logger::Init();

    BFGE_CORE_INFO("Initializing Engine");
    // Engine initialization
    BFGe::Engine* engine = new BFGe::Engine(argc, argv);

    BFGE_CORE_INFO("Initializing Application");
    // Application entrypoint (defined by client)
    BFGe::Application* app = BFGe::CreateApplication(engine);
    app->onInit();
    engine->RunApp(app);

    BFGE_CORE_INFO("Tearing down app and engine");
    // Engine teardown
    delete app;
    delete engine;

    return 0;
}
