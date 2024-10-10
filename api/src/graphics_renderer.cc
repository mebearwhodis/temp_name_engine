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

SDL_Renderer* GraphicsRenderer::GetSDLRenderer()
{
    return renderer_;
}
