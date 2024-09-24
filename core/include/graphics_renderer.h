#ifndef KUMAENGINE_CORE_GRAPHICS_RENDERER_H_
#define KUMAENGINE_CORE_GRAPHICS_RENDERER_H_

//#include <SDL2/SDL.h>

#include "../../cmake-build-debug/vcpkg_installed/x64-windows/include/SDL2/SDL.h"
//#include "../../cmake-build-debug/vcpkg_installed/x64-windows/include/SDL2/SDL_render.h"

class GraphicsRenderer
{
public:
    GraphicsRenderer(SDL_Window* window);
    ~GraphicsRenderer();

    void Clear();
    void Present();
    void DrawRectangle(int x, int y, int width, int height);

private:
    SDL_Renderer* renderer_;
};

#endif // KUMAENGINE_CORE_GRAPHICS_RENDERER_H_
