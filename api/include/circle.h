#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"


class Circle final : public Shape {

private:
    int radius_;
    math::Vec2i orbit_center_;
    float orbit_radius_;
    float orbit_angle_;
    float orbit_speed_;

public:
    // Constructor
    Circle(math::Vec2i center, int radius, math::Vec2i orbit_center, float orbit_radius, float speed, SDL_Color color);

    void UpdateOrbit();

    void Draw(SDL_Renderer* renderer) override;
};


#endif //CIRCLE_H
