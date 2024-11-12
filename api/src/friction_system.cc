#include "friction_system.h"

#include <iostream>
#include <ranges>

#include "random.h"
#include "physics/contact_solver.h"

FrictionSystem::FrictionSystem()
{
    quadtree_ = new physics::Quadtree(math::AABB(math::Vec2f(0, 0), math::Vec2f(1200, 800)));
    objects_.reserve(100);
    math::AABB ground(math::Vec2f(360.0f, 700.0f), math::Vec2f(840.0f, 600.0f));
    size_t i = objects_.size();
    CreateGround(i, ground);
}


void FrictionSystem::SpawnShape(const math::Vec2f pos, const math::ShapeType type)
{
    size_t i = objects_.size();

    switch (type)
    {
    case math::ShapeType::kAABB:
        {
            math::AABB aabb(pos);
            CreateObject(i, aabb);
        }
        break;
    case math::ShapeType::kCircle:
        {
            const float radius = random::Range(5.f, 20.f);
            math::Circle circle(pos, radius);
            CreateObject(i, circle);
        }
        break;
    case math::ShapeType::kPolygon:
    case math::ShapeType::kNone:
    default:
        break;
    }
}

void FrictionSystem::CreateObject(size_t index, math::Circle& circle)
{
    math::Vec2f velocity(0.0f, 0.0f);
    physics::Body body(circle.centre(), velocity, random::Range(1.0f, 100.0f));
    physics::Collider collider(circle, random::Range(0.5f, 0.5f), 0, false);
    GameObject object(body, collider, circle.radius());

    objects_.push_back(object);
    RegisterObject(objects_[index]);
}

void FrictionSystem::CreateObject(size_t index, math::AABB& aabb)
{
    math::Vec2f velocity(0.0f, 0.0f);
    physics::Body body(aabb.GetCenter(), velocity, random::Range(1.0f, 100.0f));
    physics::Collider collider(aabb, random::Range(0.0f, 0.0f), 0, false);
    GameObject object(body, collider, aabb.half_size_length());

    objects_.push_back(object);
    RegisterObject(objects_[index]);
}

void FrictionSystem::CreateGround(size_t index, math::AABB& aabb)
{
    math::Vec2f velocity(0.0f, 0.0f);
    physics::Body body(aabb.GetCenter(), velocity, 0.0f);
    body.set_is_static(true);
    physics::Collider collider(aabb, 1.0f, 0, false);
    GameObject object(body, collider, aabb.half_size_length());

    objects_.push_back(object);
    RegisterObject(objects_[index]);
}

void FrictionSystem::RegisterObject(GameObject& object)
{
    collider_to_object_map_[&object.collider()] = &object;
}

void FrictionSystem::UnregisterObject(GameObject& object)
{
    collider_to_object_map_.erase(&object.collider());
}

void FrictionSystem::Update()
{
    UpdateShapes();
    BroadPhase();
    NarrowPhase();
}

void FrictionSystem::UpdateShapes()
{
    for (auto& object : objects_)
    {
        auto& body = object.body();
        auto& collider = object.collider();

        //TODO change for actual delta-time
        body.Update(1.0f / 60.0f);
        math::Vec2f gravity(0.0f, 9.8f);
        if(!body.is_static())
        {
            body.ApplyForce(gravity);

            auto position = body.position();

            float radius = object.radius();

            //Check for collision with window borders
            if (position.x - radius < 0 || position.x + radius > 1200)
            {
                body.set_velocity(math::Vec2f(-body.velocity().x, body.velocity().y));
            }
            if (position.y - radius < 0 || position.y + radius > 800)
            {
                body.set_velocity(math::Vec2f(body.velocity().x, -body.velocity().y));
            }

            //Update the collider's position
            collider.set_shape(std::visit([&position](auto shape) -> std::variant<math::Circle, math::AABB, math::Polygon>
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, math::Circle>)
                {
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
}

void FrictionSystem::BroadPhase()
{
    std::unordered_map<GameObjectPair, bool> new_potential_pairs;

    quadtree_->Clear();
    for (auto& object : objects_)
    {
        quadtree_->Insert(&object.collider());
    }

    // Use AABB tests for broad phase
    for (auto& object : objects_)
    {
        auto& collider = object.collider();
        // Get the AABB of the collider for broad phase test
        auto range = collider.GetBoundingBox();
        auto potentialColliders = quadtree_->Query(range);

        for (auto& otherCollider : potentialColliders)
        {
            if (&collider != otherCollider)
            {
                // Avoid self-collision
                // Only test AABB overlap in broad phase
                if (math::Intersect(range, otherCollider->GetBoundingBox()))
                {
                    GameObject* objectA = collider_to_object_map_[&collider];
                    GameObject* objectB = collider_to_object_map_[otherCollider];
                    if (objectA && objectB)
                    {
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

void FrictionSystem::NarrowPhase()
{
    std::unordered_map<GameObjectPair, bool> newActivePairs;

    for (const auto& pair : potential_pairs_ | std::views::keys)
    {
        if (!pair.gameObjectA_ || !pair.gameObjectB_)
        {
            continue;
        }

        bool intersect = std::visit([](auto&& shape_a, auto&& shape_b)
                                    {
                                        return math::Intersect(shape_a, shape_b);
                                    }, pair.gameObjectA_->collider().shape(),
                                    pair.gameObjectB_->collider().shape());

        if (intersect)
        {
            newActivePairs[pair] = true;

            // If this is a new collision
            if (!active_pairs_.contains(pair))
            {
                OnPairCollide(pair);
            }
        }
    }

    // Check for ended collisions
    for (const auto& pair : active_pairs_ | std::views::keys)
    {
        if (!newActivePairs.contains(pair))
        {
            OnPairCollideEnd(pair);
        }
    }
    active_pairs_ = std::move(newActivePairs);
}

//Called on the first collision frame
void FrictionSystem::OnPairCollide(const GameObjectPair& pair)
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
void FrictionSystem::OnPairCollideEnd(const GameObjectPair& pair)
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
