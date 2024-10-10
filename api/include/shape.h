#ifndef SHAPE_H
#define SHAPE_H
#include <SDL_pixels.h>
#include <SDL_render.h>


class Shape {
protected:
    int x_;
    int y_;
    SDL_Color color_;
    virtual void Draw(SDL_Renderer* renderer) = 0;

public:
    Shape(int x, int y, SDL_Color color);
};



#endif //SHAPE_H
