#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"


class Rectangle : public Shape {
private:
    int width_;
    int height_;
public:
    Rectangle(int centerX, int centerY, int width, int height, SDL_Color color);
    void Draw(SDL_Renderer* renderer) override;
};



#endif //RECTANGLE_H
