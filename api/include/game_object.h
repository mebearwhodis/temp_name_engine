#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <SDL_pixels.h>

#include "physics/body.h"
#include "physics/collider.h"

class GameObject
{
private:
    physics::Body body_;
    physics::Collider collider_;
    float radius_ = 0.f;
    SDL_Color color_ = SDL_Color{ 255, 13, 132, 255 };

public:
    GameObject(const physics::Body& body, const physics::Collider& collider, const float radius) : body_(body), collider_(collider), radius_(radius) {}
    ~GameObject() = default;

    [[nodiscard]] physics::Body& body() { return body_; }
    [[nodiscard]] physics::Collider& collider() { return collider_; }
    [[nodiscard]] float radius() const { return radius_; }
    [[nodiscard]] SDL_Color color() const { return color_; }
    [[nodiscard]] math::Vec2f position() const { return body_.position(); }

    void set_body(const physics::Body& body) { body_ = body; }
    void set_collider(const physics::Collider& collider) { collider_ = collider; }
    void set_radius(const float radius) { radius_ = radius; }
    void set_color(const SDL_Color& color) { color_ = color; }
};
#endif //GAME_OBJECT_H
