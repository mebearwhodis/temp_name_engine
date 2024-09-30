#ifndef KUMAENGINE_API_GRAPHICS_RENDERER_H_
#define KUMAENGINE_API_GRAPHICS_RENDERER_H_

#include <SDL2/SDL_render.h>

class GraphicsRenderer
{
public:
    GraphicsRenderer(SDL_Window* window);
    ~GraphicsRenderer();

    void Clear();
    void Draw();
    void DrawRectangle(int x, int y, int width, int height, SDL_Color color);

private:
    SDL_Renderer* renderer_;
};

#endif // KUMAENGINE_API_GRAPHICS_RENDERER_H_
