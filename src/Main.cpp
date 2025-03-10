#include "Core/Application.h"

int main() 
{
    Athena::Application* app = new Athena::Application();
    app->Run();
    delete app;
}
