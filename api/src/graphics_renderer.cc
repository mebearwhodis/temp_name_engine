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

void GraphicsRenderer::DrawCircle(int centerX, int centerY, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);

    for(int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; //Horizontal offset
            int dy = radius - h; //Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer_, centerX + dx, centerY + dy);
            }
        }
    }
}

SDL_Renderer* GraphicsRenderer::GetSDLRenderer()
{
    return renderer_;
}
