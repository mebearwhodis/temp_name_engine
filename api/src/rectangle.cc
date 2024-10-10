#include "rectangle.h"

Rectangle::Rectangle(int centerX, int centerY, int width, int height, SDL_Color color): Shape(centerX, centerY, color)
{
    width_ = width;
    height_ = height;
}

void Rectangle::Draw(SDL_Renderer* renderer)
{
    SDL_Rect rect = {x_, y_, width_, height_};
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect);
}
