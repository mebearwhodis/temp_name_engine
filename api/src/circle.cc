#include "circle.h"

#include <numbers>
#include <SDL_render.h>

Circle::Circle(core::Vec2i center, int radius, core::Vec2i orbit_center, float orbit_radius, float speed, SDL_Color color): Shape(center, color)
{
    radius_ = radius;
    orbit_center_ = orbit_center;
    orbit_radius_ = orbit_radius;
    orbit_speed_ = speed;
}

void Circle::UpdateOrbit()
{
    pos_.x = static_cast<int>(orbit_center_.x + orbit_radius_ * cos(orbit_angle_));
    pos_.y = static_cast<int>(orbit_center_.y + orbit_radius_ * cos(orbit_angle_));

    orbit_angle_ += orbit_speed_;
    if(orbit_angle_ >= 2 * std::numbers::pi_v<float>)
    {
        orbit_angle_ -= 2 * std::numbers::pi_v<float>;
    }
}

void Circle::Draw(SDL_Renderer* renderer)
{
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

