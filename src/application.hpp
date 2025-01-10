namespace BFGengine {
    class Application {
    public:
        virtual ~Application();

        void Run();

        virtual void onInit() = 0;
        virtual void onTick() = 0;
    };

    Application* CreateApplication();
}
