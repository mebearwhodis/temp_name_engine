#ifndef KUMA_ENGINE_API_IMGUI_H_
#define KUMA_ENGINE_API_IMGUI_H_

#include <imgui_impl_sdl2.h>

#include "display.h"

class GameEngine;

class ImGuiInterface
{
private:
    GameEngine* game_engine_;
    bool show_quadtree_ = true;
    float speed_multiplier_ = 1.0f;
public:
    ImGuiInterface() = default;
    ~ImGuiInterface();

    void Initialize(Display* display, GameEngine* engine);
    void Update(bool& show_imgui);
    void Render();
    void PassEvents(SDL_Event& event);

    [[nodiscard]] bool show_quadtree() const { return show_quadtree_; }
    [[nodiscard]] float speed_multiplier() const { return speed_multiplier_; }
};

#endif // KUMA_ENGINE_API_IMGUI_H_
