#ifndef BODY_H
#define BODY_H
#include "math/vec2.h"

namespace physics
{
class Body
{
private:
    math::Vec2f position_ = math::Vec2f::Zero();
    math::Vec2f velocity_ = math::Vec2f::Zero();
    math::Vec2f acceleration_ = math::Vec2f::Zero();
    float mass_ = 1.0f;
    float inverse_mass_ = 1.0f;

public:
    Body() = default;
    Body(const math::Vec2f position, const math::Vec2f velocity, const float mass)
    {
        position_ = position;
        velocity_ = velocity;
        mass_ = mass;
        if(mass_ == 0.0f)
        {
            inverse_mass_ = 0.0f;
        }
        else
        {
            inverse_mass_ = 1.0f / mass;
        }
    };

    //Getters
    [[nodiscard]] math::Vec2f position() const { return position_; }
    [[nodiscard]] math::Vec2f velocity() const { return velocity_; }
    [[nodiscard]] math::Vec2f acceleration() const { return acceleration_; }
    [[nodiscard]] float mass() const { return mass_; }
    [[nodiscard]] float inverse_mass() const { return inverse_mass_; }
    // [[nodiscard]] BodyType type() const { return type_; }

    //Setters
    void set_position(const math::Vec2f new_position) { position_ = new_position; }
    void set_velocity(const math::Vec2f new_velocity) { velocity_ = new_velocity; }
    void set_mass(const float new_mass) { mass_ = new_mass; }
    // void set_type(const BodyType new_type) { type_ = new_type; }

    void ApplyForce(const math::Vec2f force) { acceleration_ += force * inverse_mass_; }
    void ApplyImpulse(const math::Vec2f impulse) { velocity_ += impulse * inverse_mass_; }

    void Update(const float delta_time)
    {
        velocity_ += acceleration_ * delta_time;
        position_ += velocity_ * delta_time;
        ResetForce();
    }

    void ResetForce() { acceleration_ = math::Vec2f::Zero(); }
};
}
#endif //BODY_H
