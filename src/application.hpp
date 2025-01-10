namespace BFGengine {
    class Application {
    public:
        virtual ~Application() = 0;

        virtual void onInit() = 0;
        virtual void onTick() = 0;
    };

    // Defined by the client (consumer of the engine)
    // returns a concrete class that conforms to the above interface
    Application* CreateApplication();
}
