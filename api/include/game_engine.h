#ifndef KUMAENGINE_API_GAME_ENGINE_H_
#define KUMAENGINE_API_GAME_ENGINE_H_

#include <vector>

#include "circle.h"
#include "SDL2/SDL.h"
#include "window.h"
#include "graphics_renderer.h"
#include "rectangle.h"

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    void Run();
private:
    bool is_running_;
    bool show_test_window_;
    Window* window_;
    GraphicsRenderer* renderer_;

    std::vector<Circle> circles_;
    std::vector<Rectangle> rectangles_;

    void HandleEvents();
};

#endif // KUMAENGINE_API_GAME_ENGINE_H_
