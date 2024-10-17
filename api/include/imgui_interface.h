#ifndef IMGUI_H
#define IMGUI_H

#include <./imgui_impl_sdl2.h>
#include <./imgui_impl_sdlrenderer2.h>

#include "graphics_renderer.h"
#include "window.h"

class ImGuiInterface
{
public:
    void Initialize(Window* window, GraphicsRenderer* renderer);
    void Update(bool& show_imgui);
    void Render();
    void PassEvents(SDL_Event& event);

    ~ImGuiInterface();
};

#endif //IMGUI_H
