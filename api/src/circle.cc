#include "circle.h"

#include <numbers>
#include <SDL_render.h>

Circle::Circle(const math::Vec2i center, const int radius, const math::Vec2i orbit_center, const float orbit_radius, const float angular_velocity, const SDL_Color color): Shape(center, color)
{
    radius_ = radius;
    orbit_center_ = orbit_center;
    orbit_radius_ = orbit_radius;
    angular_velocity_ = angular_velocity;
}

void Circle::UpdateOrbit()
{
    //TODO position & force rather than angle and radius (can have a radius and angle to spawn them but then convert in a position)
    //and have the 'planet' or sun at the center apply a force on it
    pos_.x = static_cast<int>(orbit_center_.x + orbit_radius_ * std::cos(orbit_angle_));
    pos_.y = static_cast<int>(orbit_center_.y + orbit_radius_ * std::sin(orbit_angle_));

    orbit_angle_ += angular_velocity_;
    if(orbit_angle_ >= 2 * std::numbers::pi_v<float>)
    {
        orbit_angle_ -= 2 * std::numbers::pi_v<float>;
    }
}

void Circle::Draw(SDL_Renderer* renderer)
{
    //TODO Make circles with triangles rather than points with SDL_RenderGeometry
    // ((see splash online main samples 02_planets.cpp))
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

    for(int w = 0; w < radius_ * 2; w++)
    {
        for (int h = 0; h < radius_ * 2; h++)
        {
            int dx = radius_ - w; //Horizontal offset
            int dy = radius_ - h; //Vertical offset
            if ((dx * dx + dy * dy) <= (radius_ * radius_))
            {
                SDL_RenderDrawPoint(renderer, pos_.x + dx, pos_.y + dy);
            }
        }
    }
}

