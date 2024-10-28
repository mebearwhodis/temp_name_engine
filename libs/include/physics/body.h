#ifndef BODY_H
#define BODY_H
#include "math/vec2.h"

namespace physics
{
enum class BodyType
{
    Static,
    Kinematic,
    Dynamic
};

class Body
{
private:
    math::Vec2f position_ = math::Vec2f::Zero();
    math::Vec2f velocity_ = math::Vec2f::Zero();
    math::Vec2f acceleration_ = math::Vec2f::Zero();
    float mass_ = 1.0f;
    BodyType type_ = BodyType::Static;

public:
    Body() = default;
    Body(math::Vec2f position, math::Vec2f velocity, float mass)
    {
        position_ = position;
        velocity_ = velocity;
        mass_ = mass;
    };

    //Getters
    [[nodiscard]] math::Vec2f position() const { return position_; }
    [[nodiscard]] math::Vec2f velocity() const { return velocity_; }
    [[nodiscard]] math::Vec2f acceleration() const { return acceleration_; }
    [[nodiscard]] float mass() const { return mass_; }
    [[nodiscard]] BodyType type() const { return type_; }

    //Setters
    void set_position(const math::Vec2f new_position) { position_ = new_position; }
    void set_velocity(const math::Vec2f new_velocity) { velocity_ = new_velocity; }
    void set_mass(const float new_mass) { mass_ = new_mass; }
    void set_type(const BodyType new_type) { type_ = new_type; }

    void ApplyForce(const math::Vec2f force) { acceleration_ += force / mass_; }

    void ResetForce() { acceleration_ = math::Vec2f::Zero(); }
};
}
#endif //BODY_H
