#ifndef KUMAENGINE_API_GAME_ENGINE_H_
#define KUMAENGINE_API_GAME_ENGINE_H_


#include "display.h"
#include "imgui_interface.h"
#include "planet_system.h"
#include "graphics_manager.h"
#include "timer.h"
#include "trigger_system.h"


class GameEngine
{
private:
    int selected_system_= 1; //This will change, it's a temporary way to switch systems
    bool is_running_;

    Display* display_;
    Timer* timer_;
    GraphicsManager* graphics_manager_;
    PlanetSystem* planet_system_;
    TriggerSystem* trigger_system_;

    ImGuiInterface* imgui_interface_;

    void HandleEvents();

public:
    GameEngine();
    ~GameEngine();

    void Run();
};

#endif // KUMAENGINE_API_GAME_ENGINE_H_
