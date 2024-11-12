#ifndef CONTACT_SOLVER_H
#define CONTACT_SOLVER_H
#include <array>
#include <utility>

#include "game_object.h"

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
                    HandleAABBCircleCollision();
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
                    std::swap(objects_[0], objects_[1]);
                    HandleAABBPolygonCollision();
                    break;
                case math::ShapeType::kCircle:
                    std::swap(objects_[0], objects_[1]);
                    HandleCirclePolygonCollision();
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

            //Calculate the relative velocity between the bodies
            const math::Vec2f relative_velocity = body_a.velocity() - body_b.velocity();

            //Relative velocity along Normal
            const float separating_velocity = math::Vec2f::Dot(relative_velocity, contact_normal_);



            if (separating_velocity > 0.0f){ return; }

            //TODO: may use a combined restitution taking the mass into account to have a more accurate simulation
            //Calculate restitution
            float restitution = std::min(objects_[0]->collider().bounciness(), objects_[1]->collider().bounciness());

            // const auto finalSepVel = -separating_velocity * restitution;
            // const auto deltaVel = finalSepVel - separating_velocity;
            //
            // const auto total_inverse_mass = body_a.inverse_mass() + body_b.inverse_mass();
            // const auto impulse = deltaVel / total_inverse_mass;
            // const auto impulsePerMass = contact_normal_ * impulse;
            // body_a.ApplyImpulse(impulsePerMass);
            // body_b.ApplyImpulse(-impulsePerMass);

            //Calculate impulse scalar
            float impulse_magnitude = -(1.0f + restitution) * separating_velocity;
            impulse_magnitude /= (body_a.inverse_mass() + body_b.inverse_mass());


            //TODO: add check if bodies are static or dynamic
            //Apply impulse
            const math::Vec2f impulse = impulse_magnitude * contact_normal_;
            body_a.ApplyImpulse(impulse);
            body_b.ApplyImpulse(-impulse);

        }

        void ResolvePositions() const
        {
            if(penetration_ <= 0.0f) {return;}
            auto& body_a = objects_[0]->body();
            auto& body_b = objects_[1]->body();
            const auto inverse_mass_a = body_a.inverse_mass();
            const auto inverse_mass_b = body_b.inverse_mass();
            const auto total_inverse_mass = inverse_mass_a + inverse_mass_b;
            constexpr float correction_percent = 0.4f;
            constexpr float slop = 0.01f; // usually 0.01 to 0.1

            const math::Vec2f correction = std::max(penetration_ - slop, 0.0f) / total_inverse_mass * correction_percent * contact_normal_;
            //const math::Vec2f correction = penetration_ / total_inverse_mass * contact_normal_;
            body_a.set_position(body_a.position() + inverse_mass_a * correction);
            body_b.set_position(body_b.position() - inverse_mass_b * correction);
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
                    contact_normal_ = centre_a.x<centre_b.x ? math::Vec2f(-1, 0) : math::Vec2f(1, 0);
                }
                else
                {
                    penetration_ = overlap_y;
                    contact_normal_ = centre_a.y<centre_b.y ? math::Vec2f(0, -1) : math::Vec2f(0, 1);
                }

                //Calculate the contact point as the midpoint of the overlapping edges
                contact_point_ = {
                    (centre_a.x + centre_b.x) / 2,
                    (centre_a.y + centre_b.y) / 2
                };
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

            contact_normal_ = delta.Normalized();
            penetration_ = radius - distance;
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
#endif //CONTACT_SOLVER_H
