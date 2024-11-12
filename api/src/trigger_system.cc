#include "trigger_system.h"

#include <iostream>
#include <ranges>

#include "random.h"
#include "physics/contact_solver.h"

TriggerSystem::TriggerSystem()
{
    quadtree_ = new physics::Quadtree(math::AABB(math::Vec2f(0,0), math::Vec2f(1200,800)));
    for(size_t i = 0; i < kNumberOfShapes/2 - 1; i++)
    {
        math::Vec2f position(random::Range(100.f, 1100.f), random::Range(100.f, 700.f));
        const float radius = random::Range(5.f, 20.f);
        math::Circle circle(position, radius);
        CreateObject(i, circle);
    }
    for(size_t i = kNumberOfShapes/2 - 1; i < kNumberOfShapes; i++)
    {
        math::Vec2f position(random::Range(100.f, 1100.f), random::Range(100.f, 700.f));
        const float half_size_value = random::Range(5.f, 20.f);
        math::AABB aabb(position, half_size_value);
        CreateObject(i, aabb);
    }
}

void TriggerSystem::CreateObject(size_t index, math::Circle& circle)
{
    math::Vec2f velocity(random::Range(-0.1f, 0.1f), random::Range(-0.1f, 0.1f));
    physics::Body body(circle.centre(), velocity, random::Range(1.0f, 100.0f));
    physics::Collider collider(circle, random::Range(1.0f, 1.0f), 0, false);
    GameObject object(body, collider, circle.radius());

    objects_[index] = object;
    RegisterObject(objects_[index]);
}

void TriggerSystem::CreateObject(size_t index, math::AABB& aabb)
{
    math::Vec2f velocity(random::Range(-0.1f, 0.1f), random::Range(-0.1f, 0.1f));
    physics::Body body(aabb.GetCenter(), velocity, random::Range(1.0f, 100.0f));
    physics::Collider collider(aabb, random::Range(1.0f, 1.0f), 0, false);
    GameObject object(body, collider, aabb.half_size());

    objects_[index] = object;
    RegisterObject(objects_[index]);
}

void TriggerSystem::RegisterObject(GameObject& object)
{
    collider_to_object_map_[&object.collider()] = &object;
}

void TriggerSystem::UnregisterObject(GameObject& object)
{
    collider_to_object_map_.erase(&object.collider());
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

        //Update the collider's position
        collider.set_shape(std::visit([&position](auto shape) -> std::variant<math::Circle, math::AABB, math::Polygon> {
            if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, math::Circle>) {
                shape.set_centre(position);
            }
            if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, math::AABB>)
            {
                shape.UpdatePosition(position);
            }
            return shape;
        }, collider.shape()));
    }
}

void TriggerSystem::SimplisticBroadPhase()
{
    //Reset all objects to the default color
    for (auto& object : objects_)
    {
        object.set_color(SDL_Color{ 255, 13, 132, 255 });
    }

    for(int i = 0; i < objects_.size(); i++)
    {
        for(int j = i + 1; j < objects_.size(); j++)
        {
            auto& collider_a = objects_[i].collider();
            auto& collider_b = objects_[j].collider();
            bool intersects = std::visit([](auto&& shape_a, auto&& shape_b)
            {
                return math::Intersect(shape_a, shape_b);
            }, collider_a.shape(), collider_b.shape());

            if(intersects)
            {
                objects_[i].set_color(SDL_Color{ 0, 255, 0, 255 });
                objects_[j].set_color(SDL_Color{ 0, 255, 0, 255 });
            }
        }
    }
}

void TriggerSystem::BroadPhase() {
    std::unordered_map<GameObjectPair, bool> new_potential_pairs;

    quadtree_->Clear();
    for (auto& object : objects_) {
        quadtree_->Insert(&object.collider());
    }

    // Use AABB tests for broad phase
    for (auto& object : objects_) {
        auto& collider = object.collider();
        // Get the AABB of the collider for broad phase test
        auto range = collider.GetBoundingBox();
        auto potentialColliders = quadtree_->Query(range);

        for (auto& otherCollider : potentialColliders) {
            if (&collider != otherCollider) {  // Avoid self-collision
                // Only test AABB overlap in broad phase
                if (math::Intersect(range, otherCollider->GetBoundingBox())) {
                    GameObject* objectA = collider_to_object_map_[&collider];
                    GameObject* objectB = collider_to_object_map_[otherCollider];
                    if (objectA && objectB) {
                        GameObjectPair pair{objectA, objectB};
                        new_potential_pairs[pair] = true;
                    }
                }
            }
        }
    }

    // Update the potential pairs for narrow phase to process
    potential_pairs_ = std::move(new_potential_pairs);
}

void TriggerSystem::NarrowPhase() {
    std::unordered_map<GameObjectPair, bool> newActivePairs;

    for (const auto& pair : potential_pairs_ | std::views::keys) {
        if (!pair.gameObjectA_ || !pair.gameObjectB_) {
            continue;
        }

        bool intersect = std::visit([](auto&& shape_a, auto&& shape_b) {
            return math::Intersect(shape_a, shape_b);
        }, pair.gameObjectA_->collider().shape(),
           pair.gameObjectB_->collider().shape());

        if (intersect) {
            newActivePairs[pair] = true;

            // If this is a new collision
            if (!active_pairs_.contains(pair)) {
                OnPairCollide(pair);
            }
        }
    }

    // Check for ended collisions
    for (const auto& [pair, _] : active_pairs_) {
        if (!newActivePairs.contains(pair)) {
            OnPairCollideEnd(pair);
        }
    }
    active_pairs_ = std::move(newActivePairs);
}

//Called on the first collision frame
void TriggerSystem::OnPairCollide(const GameObjectPair& pair)
{
    if (pair.gameObjectA_->collider().is_trigger() || pair.gameObjectB_->collider().is_trigger())
    {
        pair.gameObjectA_->OnTriggerEnter();
        pair.gameObjectB_->OnTriggerEnter();
    }
    else
    {
        physics::ContactSolver ContactSolver;
        ContactSolver.SetContactObjects(pair);
        ContactSolver.ResolveContact();
        pair.gameObjectA_->OnCollisionEnter();
        pair.gameObjectB_->OnCollisionEnter();
    }
}

//TODO: find a way to check if it's still colliding with something else when Exit
void TriggerSystem::OnPairCollideEnd(const GameObjectPair& pair)
{
    if (pair.gameObjectA_->collider().is_trigger() || pair.gameObjectB_->collider().is_trigger())
    {
        pair.gameObjectA_->OnTriggerExit();
        pair.gameObjectB_->OnTriggerExit();
    }
    else
    {
        pair.gameObjectA_->OnCollisionExit();
        pair.gameObjectB_->OnCollisionExit();
    }
}


