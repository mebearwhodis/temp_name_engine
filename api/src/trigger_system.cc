#include "trigger_system.h"

#include <iostream>

#include "random.h"

TriggerSystem::TriggerSystem()
{
    quadtree_ = new physics::Quadtree();
    for(size_t i = 0; i < starting_number_of_shapes_; i++)
    {
        math::Vec2f position(random::Range(100.f, 1100.f), random::Range(100.f, 700.f));
        float radius = random::Range(5.f, 20.f);
        math::Circle circle(position, radius);
        CreateObject(circle);
    }
}

void TriggerSystem::CreateObject(math::Circle& circle)
{
    math::Vec2f velocity(random::Range(-0.05f, 0.05f), random::Range(-0.05f, 0.05f));
    physics::Body body(circle.center(), velocity, 0);
    physics::Collider collider(circle, 0, 0, true);
    GameObject object(body, collider, circle.radius());
    objects_.push_back(object);
}

void TriggerSystem::Update()
{
    UpdateShapes();
    BroadPhase();
    NarrowPhase();
}

void TriggerSystem::UpdateShapes()
{
    for (auto& object : objects_)
    {
        auto& body = object.body();
        auto& collider = object.collider();
        auto position = body.position();
        auto velocity = body.velocity();

        //Update position based on velocity
        position += velocity;

        float radius = object.radius();

        //Check for collision with window borders
        if (position.x - radius < 0 || position.x + radius > 1200)
        {
            velocity.x *= -1; //Reverse x velocity on horizontal collision
        }
        if (position.y - radius < 0 || position.y + radius > 800)
        {
            velocity.y *= -1; //Reverse y velocity on vertical collision
        }

        //Set updated position and velocity
        body.set_position(position);
        body.set_velocity(velocity);

        // Update the collider's position
        collider.set_shape(std::visit([&position](auto shape) -> std::variant<math::Circle, math::AABB, math::Polygon> {
            if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, math::Circle>) {
                shape.set_center(position);
            }
            // Add cases for other shape types if needed
            return shape;
        }, collider.shape()));
    }
}

void TriggerSystem::BroadPhase()
{
    // //Reset all objects to the default color
    // for (auto& object : objects_)
    // {
    //     object.set_color(SDL_Color{ 255, 13, 132, 255 });
    // }
    //
    // for(int i = 0; i < objects_.size(); i++)
    // {
    //     for(int j = i + 1; j < objects_.size(); j++)
    //     {
    //         auto& collider_a = objects_[i].collider();
    //         auto& collider_b = objects_[j].collider();
    //         bool intersects = std::visit([](auto&& shape_a, auto&& shape_b)
    //         {
    //             return math::Intersect(shape_a, shape_b);
    //         }, collider_a.shape(), collider_b.shape());
    //
    //         if(intersects)
    //         {
    //             objects_[i].set_color(SDL_Color{ 0, 255, 0, 255 });
    //             objects_[j].set_color(SDL_Color{ 0, 255, 0, 255 });
    //         }
    //     }
    // }
}

void TriggerSystem::NarrowPhase()
{
}
