#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"


class Circle final : public Shape {

private:
    int radius_;
    math::Vec2i orbit_center_;
    float orbit_radius_;
    float orbit_angle_;
    float angular_velocity_;

public:
    // Constructor
    Circle(const math::Vec2i center, const int radius, const math::Vec2i orbit_center, const float orbit_radius, const float angular_velocity, const SDL_Color color);

    void UpdateOrbit();

    void Draw(SDL_Renderer* renderer) override;
};


#endif //CIRCLE_H
