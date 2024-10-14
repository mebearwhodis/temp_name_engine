#ifndef SHAPE_H
#define SHAPE_H
#include <SDL_pixels.h>
#include <SDL_render.h>

#include "maths/vec2.h"


class Shape {
protected:
    core::Vec2i pos_;
    SDL_Color color_;
    virtual void Draw(SDL_Renderer* renderer) = 0;

public:
    Shape(core::Vec2i position, SDL_Color color);

    void set_color(SDL_Color color);
    void set_position(core::Vec2i position);

};



#endif //SHAPE_H
