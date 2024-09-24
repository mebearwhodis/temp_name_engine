#ifndef KUMAENGINE_CORE_GAME_ENGINE_H_
#define KUMAENGINE_CORE_GAME_ENGINE_H_

//#include <SDL2/SDL.h>
#include "../../cmake-build-debug/vcpkg_installed/x64-windows/include/SDL2/SDL.h"
#include "window.h"
#include "graphics_renderer.h"

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    void Run();
private:
    bool is_running_;
    Window* window_;
    GraphicsRenderer* renderer_;

    void HandleEvents();
};

#endif // KUMAENGINE_CORE_GAME_ENGINE_H_
