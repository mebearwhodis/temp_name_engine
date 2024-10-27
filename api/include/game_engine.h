#ifndef KUMAENGINE_API_GAME_ENGINE_H_
#define KUMAENGINE_API_GAME_ENGINE_H_

#include <vector>

#include "display.h"
#include "imgui_interface.h"
#include "planet_system.h"
#include "graphical_shape.h"

class GameEngine
{
private:
    bool is_running_;

    Display* display_;
    GraphicalShape* shape_manager_;
    PlanetSystem* planet_system_;

    ImGuiInterface* imgui_interface_;

    void HandleEvents();

public:
    GameEngine();
    ~GameEngine();

    void Run();
};

#endif // KUMAENGINE_API_GAME_ENGINE_H_
