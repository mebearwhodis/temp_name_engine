#include "circle.h"

#include <SDL_render.h>

Circle::Circle(int centerX, int centerY, int radius, SDL_Color color): Shape(centerX, centerY, color)
{
    radius_ = radius;
}

void Circle::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

    for(int w = 0; w < radius_ * 2; w++)
    {
        for (int h = 0; h < radius_ * 2; h++)
        {
            int dx = radius_ - w; //Horizontal offset
            int dy = radius_ - h; //Vertical offset
            if ((dx * dx + dy * dy) <= (radius_ * radius_))
            {
                SDL_RenderDrawPoint(renderer, x_ + dx, y_ + dy);
            }
        }
    }
}
