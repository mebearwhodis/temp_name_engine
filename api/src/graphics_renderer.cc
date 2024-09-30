#include "graphics_renderer.h"

GraphicsRenderer::GraphicsRenderer(SDL_Window* window)
{
    renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

GraphicsRenderer::~GraphicsRenderer()
{
    SDL_DestroyRenderer(renderer_);
}

void GraphicsRenderer::Clear()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void GraphicsRenderer::Draw()
{
    SDL_RenderPresent(renderer_);
}

void GraphicsRenderer::DrawRectangle(int x, int y, int width, int height, SDL_Color color)
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer_, &rect);
}
