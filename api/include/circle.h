#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"


class Circle final : public Shape {

private:
    int radius_;
public:
    Circle(int centerX, int centerY, int radius, SDL_Color color);
    void Draw(SDL_Renderer* renderer) override;
};



#endif //CIRCLE_H
