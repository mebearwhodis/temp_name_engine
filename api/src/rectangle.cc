#include "rectangle.h"

Rectangle::Rectangle(core::Vec2i position, int width, int height, SDL_Color color): Shape(position, color)
{
    width_ = width;
    height_ = height;
}

void Rectangle::Draw(SDL_Renderer* renderer)
{
    SDL_Rect rect = {pos_.x, pos_.y, width_, height_};
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect);
}
