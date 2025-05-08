#include "Core/Memory/MemoryTracker.h"
#include "Core/Application.h"

int main() 
{
    Athena::MemoryTracker::Init();
    Athena::Application* app = new Athena::Application();
    app->Run();
    delete app;
    Athena::MemoryTracker::Shutdown();
}
