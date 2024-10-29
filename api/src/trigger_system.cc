#include "trigger_system.h"

#include <iostream>

#include "random.h"

TriggerSystem::TriggerSystem()
{
    quadtree_ = new physics::Quadtree(math::AABB(math::Vec2f(0,0), math::Vec2f(1200,800)));
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

    RegisterObject(object);
    objects_.push_back(object);
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
    // NarrowPhase();
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

    std::unordered_map<GameObjectPair, bool> newActivePairs;

    quadtree_->Clear();
    for (auto& object : objects_) {
        quadtree_->Insert(&object.collider());
    }

    for (auto& object : objects_) {
        auto& collider = object.collider();
        auto range = collider.GetBoundingBox();
        auto potentialColliders = quadtree_->Query(range);

        for (auto& otherCollider : potentialColliders) {
            if (&collider != otherCollider) {  // Avoid self-collision
                GameObject* otherObject = collider_to_object_map_[otherCollider]; // Get the corresponding GameObject
                GameObjectPair pair{&object, otherObject}; // Create the pair with GameObjects
                newActivePairs[pair] = true;
            }
        }
    }

    // Now update active_pairs_ based on newActivePairs
    for (const auto& [pair, active] : newActivePairs) {
        if (active_pairs_.find(pair) == active_pairs_.end()) {
            // New collision
            OnTriggerEnter(pair);
        }
        active_pairs_[pair] = true;
    }

    // Check for collisions that have ended
    for (auto it = active_pairs_.begin(); it != active_pairs_.end();) {
        if (newActivePairs.find(it->first) == newActivePairs.end()) {
            // Collision has ended
            OnTriggerExit(it->first);
            it = active_pairs_.erase(it);
        } else {
            ++it;
        }
    }
}

// void TriggerSystem::NarrowPhase() {
//     for (auto& pair : active_pairs_) {
//         bool intersect = std::visit([](auto&& shape_a, auto&& shape_b)
//                 {
//                     return math::Intersect(shape_a, shape_b);
//                 }, pair.first.collider_a_->shape(), pair.first.collider_b_->shape());
//         if (intersect) {
//             //OnTriggerEnter(pair.first);
//             pair.first.collider_a_.
//         } else {
//             //OnTriggerExit(pair.first);
//             std::cout << "End intersect" << std::endl;
//         }
//     }
// }

//Called on the first collision frame
void TriggerSystem::OnTriggerEnter(const GameObjectPair& pair)
{
    if (pair.gameObjectA_) {
        pair.gameObjectA_->set_color(SDL_Color{ 0, 255, 0, 255 });
    }
    if (pair.gameObjectB_) {
        pair.gameObjectB_->set_color(SDL_Color{ 0, 255, 0, 255 });
    }
}

//Called on the last collision frame
void TriggerSystem::OnTriggerExit(const GameObjectPair& pair)
{
    if (pair.gameObjectA_) {
        pair.gameObjectA_->set_color(SDL_Color{ 255, 13, 132, 255 }); // Revert to original color
    }
    if (pair.gameObjectB_) {
        pair.gameObjectB_->set_color(SDL_Color{ 255, 13, 132, 255 }); // Revert to original color
    }
}
