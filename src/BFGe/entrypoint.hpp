#include "application.hpp"
#include "BFGe.hpp"

namespace BFGe {
    // Defined by the client (consumer of the engine)
    // returns a concrete class that conforms to the above interface
    BFGe::Application* CreateApplication(BFGe::Engine* engine);
}
