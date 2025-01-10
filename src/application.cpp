#include "application.hpp"

// The destructer must be defined on the base class (even if it is an interface)
// This is due to inherited destructors not overriding the base definition,
// but instead calling the destructors in reverse up the heirarchy tree
// Thus every class must have a destructor (or so I have come to understand)
BFGengine::Application::~Application() {}
