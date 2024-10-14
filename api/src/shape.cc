#include "shape.h"

Shape::Shape(core::Vec2i position, SDL_Color color)
{
    pos_ = position;
    color_ = color;
}


void Shape::set_position(core::Vec2i position)
{
    pos_ = position;
}

void Shape::set_color(SDL_Color color)
{
    color_ = color;
}
