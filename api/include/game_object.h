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
    SDL_Color color_ = SDL_Color{255, 13, 132, 255};

public:
    GameObject() = default;
    GameObject(const physics::Body& body, const physics::Collider& collider, const float radius) : body_(body),
        collider_(collider), radius_(radius){}
    GameObject(const physics::Body& body, const float radius, const SDL_Color& color) : body_(body), radius_(radius), color_(color){}

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

    void OnTriggerEnter(){ color_ = SDL_Color{ 0, 255, 0, 255 }; }
    void OnTriggerExit(){ color_ = SDL_Color{ 255, 13, 132, 255 }; }
};

struct GameObjectPair
{
    GameObject* gameObjectA_;
    GameObject* gameObjectB_;

    bool operator==(const GameObjectPair& other) const
    {
        return (gameObjectA_ == other.gameObjectA_ && gameObjectB_ == other.gameObjectB_) ||
            (gameObjectA_ == other.gameObjectB_ && gameObjectB_ == other.gameObjectA_);
    }
};

namespace std
{
    template <>
    struct std::hash<GameObjectPair>
    {
        std::size_t operator()(const GameObjectPair& pair) const noexcept
        {
            //Hash the pointer values
            std::size_t h1 = std::hash<const GameObject*>{}(pair.gameObjectA_);
            std::size_t h2 = std::hash<const GameObject*>{}(pair.gameObjectB_);
            //Combine hashes
            return h1 ^ (h2 << 1);
        }
    };
};
#endif //GAME_OBJECT_H
