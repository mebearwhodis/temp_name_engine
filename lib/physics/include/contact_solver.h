﻿#ifndef KUMA_ENGINE_LIB_PHYSICS_CONTACT_SOLVER_H_
#define KUMA_ENGINE_LIB_PHYSICS_CONTACT_SOLVER_H_

#include <array>
#include <utility>

#include "game_object.h"
#include "shape.h"
#include "vec2.h"

namespace physics
{
    struct ContactSolver
    {
        std::array<GameObject*, 2> objects_{};

        math::Vec2f contact_point_ = math::Vec2f::Zero();
        math::Vec2f contact_normal_ = math::Vec2f::Zero();

        float penetration_ = 0.0f;

        void SetContactObjects(const GameObjectPair& pair)
        {
            objects_[0] = pair.gameObjectA_;
            objects_[1] = pair.gameObjectB_;
        }

        void ResolveContact()
        {
            CalculateProperties();
            if(objects_[0]->body().type() == BodyType::Static)
            {
                std::swap(objects_[0], objects_[1]);
                contact_normal_ = -contact_normal_;
            }
            ResolveVelocities();
            ResolvePositions();
        }

    private:
        void CalculateProperties()
        {
            const auto collider_a = objects_[0]->collider();
            const auto collider_b = objects_[1]->collider();

            //Reset properties
            contact_point_ = math::Vec2f::Zero();
            contact_normal_ = math::Vec2f::Zero();
            penetration_ = 0.0f;

            switch (collider_a.GetShapeType())
            {
            case math::ShapeType::kAABB:
                switch (collider_b.GetShapeType())
                {
                case math::ShapeType::kAABB:
                    HandleAABBAABBCollision();
                    break;
                case math::ShapeType::kCircle:
                    HandleAABBCircleCollision();
                    break;
                case math::ShapeType::kPolygon:
                    HandleAABBPolygonCollision();
                    break;
                case math::ShapeType::kNone:
                default:
                    break;
                }
                break;
            case math::ShapeType::kCircle:
                switch (collider_b.GetShapeType())
                {
                case math::ShapeType::kAABB:
                    std::swap(objects_[0], objects_[1]);
                    CalculateProperties();
                    break;
                case math::ShapeType::kCircle:
                    HandleCircleCircleCollision();
                    break;
                case math::ShapeType::kPolygon:
                    HandleCirclePolygonCollision();
                    break;
                case math::ShapeType::kNone:
                default:
                    break;
                }
                break;
            case math::ShapeType::kPolygon:
                switch (collider_b.GetShapeType())
                {
                case math::ShapeType::kAABB:
                case math::ShapeType::kCircle:
                    std::swap(objects_[0], objects_[1]);
                    CalculateProperties();
                    break;
                case math::ShapeType::kPolygon:
                    HandlePolygonPolygonCollision();
                    break;
                case math::ShapeType::kNone:
                default:
                    break;
                }
                break;
            case math::ShapeType::kNone:
            default:
                break;
            }
        }

        void ResolveVelocities() const
        {
            auto& body_a = objects_[0]->body();
            auto& body_b = objects_[1]->body();

            const auto& collider_a = objects_[0]->collider();
            const auto& collider_b = objects_[1]->collider();

            // Calculate the relative velocity between the bodies
            const math::Vec2f relative_velocity = body_a.velocity() - body_b.velocity();

            // Relative velocity along Normal
            const float separating_velocity = math::Vec2f::Dot(relative_velocity, contact_normal_);

            if (separating_velocity > 0.0f) { return; }

            // Calculate restitution
            const float restitution = (objects_[0]->collider().bounciness() * body_a.mass() +
                    objects_[1]->collider().bounciness() * body_b.mass()) /
                (body_a.mass() + body_b.mass());


            // Calculate impulse scalar
            float impulse_magnitude = -(1.0f + restitution) * separating_velocity;
            impulse_magnitude /= (body_a.inverse_mass() + body_b.inverse_mass());

            // // Debug output
            // std::cout << "ResolveVelocities:" << std::endl;
            // std::cout << "  Body A Velocity: " << body_a.velocity().x << " : " << body_a.velocity().y << std::endl;
            // std::cout << "  Body B Velocity: " << body_b.velocity().x << " : " << body_b.velocity().y << std::endl;
            // std::cout << "  Separating Velocity: " << separating_velocity << std::endl;
            // std::cout << "  Impulse Magnitude: " << impulse_magnitude << std::endl;

            // Apply impulse to the dynamic bodies
            if (body_a.type() == BodyType::Dynamic)
            {
                body_a.ApplyImpulse(impulse_magnitude * contact_normal_);
            }
            if (body_b.type() == BodyType::Dynamic)
            {
                body_b.ApplyImpulse(-impulse_magnitude * contact_normal_);
            }

            //Friction
            //Tangent vector
            const math::Vec2f tangent = (relative_velocity - separating_velocity * contact_normal_).Normalized();
            // Magnitude to apply
            const float jt = -math::Vec2f::Dot(relative_velocity, tangent) / (body_a.inverse_mass() + body_b.inverse_mass());
            const float mu = std::sqrt(
                collider_a.friction() * collider_a.friction() + collider_b.friction() * collider_b.friction());

            // Clamp friction
            math::Vec2f friction_impulse = math::Vec2f::Zero();
            if (std::abs(jt) < impulse_magnitude * mu)
            {
                friction_impulse = jt * tangent;
            }
            else
            {
                const float dynamic_friction_impulse = std::sqrt(
                    collider_a.dynamic_friction() * collider_a.dynamic_friction() + collider_b.dynamic_friction() *
                    collider_b.dynamic_friction());
                friction_impulse = -impulse_magnitude * tangent * dynamic_friction_impulse;
            }

            // Apply impulse to the dynamic bodies
            if (body_a.type() == BodyType::Dynamic)
            {
                body_a.ApplyImpulse(friction_impulse);
            }
            if (body_b.type() == BodyType::Dynamic)
            {
                body_b.ApplyImpulse(-friction_impulse);
            }
        }

        void ResolvePositions() const
        {
            if (penetration_ <= 0.0f) { return; }

            auto& body_a = objects_[0]->body();
            auto& body_b = objects_[1]->body();
            const auto inverse_mass_a = body_a.inverse_mass();
            const auto inverse_mass_b = body_b.inverse_mass();
            const auto total_inverse_mass = inverse_mass_a + inverse_mass_b;
            if(total_inverse_mass <= std::numeric_limits<float>::epsilon()) { return; }

            //Correct positions to avoid objects sinking into each other
            //Only correct position if the penetration is above this threshold
            float penetration_correction = std::max(penetration_, 0.01f);
            constexpr float correction_percent = 1.0f;
            const math::Vec2f correction = contact_normal_ * (penetration_correction * correction_percent);

            // Debug output
            // std::cout << "ResolvePositions:" << std::endl;
            // std::cout << "  Penetration: " << penetration_ << std::endl;
            // std::cout << "  Correction: " << correction.x << " : " << correction.y << std::endl;
            // std::cout << "  Body A Position Before: " << body_a.position().x << " : " << body_a.position().y << std::endl;
            // std::cout << "  Body B Position Before: " << body_b.position().x << " : " << body_b.position().y << std::endl;

            // only move the dynamic bodies
            if (body_a.type() != BodyType::Static)
            {
                body_a.set_position(body_a.position() - correction * inverse_mass_a);
            }
            if (body_b.type() != BodyType::Static)
            {
                body_b.set_position(body_b.position() + correction * inverse_mass_b) ;
            }
            // Debug output after position correction
            // std::cout << "  Body A Position After: " << body_a.position().x << " : " << body_a.position().y << std::endl;
            // std::cout << "  Body B Position After: " << body_b.position().x << " : " << body_b.position().y << std::endl;
            // std::cout << "-------------------------" << std::endl;
        }

        void HandleAABBAABBCollision()
        {
            const auto& aabb_a = std::get<math::AABB>(objects_[0]->collider().shape());
            const auto& aabb_b = std::get<math::AABB>(objects_[1]->collider().shape());
            const auto centre_a = objects_[0]->position();
            const auto centre_b = objects_[1]->position();

            //Calculate the overlap on each axis
            float overlap_x = std::min(aabb_a.max_bound().x, aabb_b.max_bound().x) - std::max(
                aabb_a.min_bound().x, aabb_b.min_bound().x);
            float overlap_y = std::min(aabb_a.max_bound().y, aabb_b.max_bound().y) - std::max(
                aabb_a.min_bound().y, aabb_b.min_bound().y);

            //Determine the smallest overlap direction
            if (overlap_x < overlap_y)
            {
                penetration_ = overlap_x;
                contact_normal_ = centre_a.x < centre_b.x ? math::Vec2f(-1, 0) : math::Vec2f(1, 0);
            }
            else
            {
                penetration_ = overlap_y;
                contact_normal_ = centre_a.y < centre_b.y ? math::Vec2f(0, -1) : math::Vec2f(0, 1);
            }

            //Calculate the contact point as the midpoint of the overlapping edges
            contact_point_ = {(centre_a.x + centre_b.x) / 2, (centre_a.y + centre_b.y) / 2};
        }

        void HandleAABBCircleCollision()
        {
            const auto& aabb = std::get<math::AABB>(objects_[0]->collider().shape());
            const auto& circle = std::get<math::Circle>(objects_[1]->collider().shape());
            const auto centre = circle.centre();
            const auto radius = circle.radius();

            //Find the closest point on the AABB to the circle centre
            const math::Vec2f closest_point = {
                std::clamp(centre.x, aabb.min_bound().x, aabb.max_bound().x),
                std::clamp(centre.y, aabb.min_bound().y, aabb.max_bound().y)
            };

            //Calculate the vector from the circle centre to the closest point
            const math::Vec2f delta = closest_point - centre;
            const float distance = delta.Magnitude();

            if (distance > std::numeric_limits<float>::epsilon())
            {
                contact_normal_ = delta / distance; // Normalized vector
                penetration_ = radius - distance;
            }
            else
            {
                // Circle center is inside the AABB
                contact_normal_ = math::Vec2f(1, 0); // Arbitrary normal
                penetration_ = radius;
            }

            // Contact point is the closest point
            contact_point_ = closest_point;

        }

        void HandleAABBPolygonCollision()
        {
            //Not Implemented
        }

        void HandleCircleCircleCollision()
        {
            const auto radius_a = objects_[0]->radius();
            const auto radius_b = objects_[1]->radius();
            const auto centre_a = objects_[0]->position();
            const auto centre_b = objects_[1]->position();

            //Calculate the vector between the centers
            const math::Vec2f delta = centre_a - centre_b;

            //Calculate contact normal and point
            contact_normal_ = delta.Normalized();
            penetration_ = radius_a + radius_b - delta.Magnitude();
            contact_point_ = centre_a + contact_normal_ * radius_a;
        }

        void HandleCirclePolygonCollision()
        {
            //Not Implemented
            //This would involve checking the closest edge of the polygon to the circle and calculating the contact point, normal, and penetration depth
        }

        void HandlePolygonPolygonCollision()
        {
            //Not Implemented
            //This would involve checking for edge intersections and calculating the contact point, normal, and penetration depth
        }
    };
}
#endif //KUMA_ENGINE_LIB_PHYSICS_CONTACT_SOLVER_H_
