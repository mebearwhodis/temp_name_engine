#include "trigger_system.h"
#include "random.h"

TriggerSystem::TriggerSystem()
{
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
    SolveBroadPhase();
    SolveNarrowPhase();
}

void TriggerSystem::UpdateShapes()
{
    for (auto& object : objects_)
    {
        auto& body = object.body();
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
    }
}

void TriggerSystem::SolveBroadPhase()
{
    for(int i = 0; i < starting_number_of_shapes_; i++)
    {
        for(int j = 0; j < starting_number_of_shapes_; j++)
        {
            // auto shape_a = objects_[i].collider().shape();
            // auto shape_b = objects_[j].collider().shape();
            // math::Intersect(shape_a, shape_b);
        }
    }
}

void TriggerSystem::SolveNarrowPhase()
{
}
