﻿#include "friction_system.h"

#include <iostream>
#include <ranges>

#include "contact_solver.h"
#include "random.h"

FrictionSystem::FrictionSystem()
{
    quadtree_ = new physics::Quadtree(math::AABB(math::Vec2f(0, 0), math::Vec2f(1200, 800)));
    timer_ = new Timer();
    objects_.reserve(1000);
    CreateGround();
}


void FrictionSystem::SpawnShape(const math::Vec2f pos, const math::ShapeType type)
{
    size_t i = objects_.size();
    math::Vec2f new_position = pos;
    const float radius = random::Range(5.f, 20.f);

    const int max_retries = 10; // Limit retries to avoid infinite loops.
    for (int retry = 0; retry < max_retries; ++retry)
    {
        bool position_valid = true;
        for (auto& object : objects_)
        {
            if (math::Intersect(object.collider().GetBoundingBox(),
                                math::AABB(new_position - math::Vec2f(radius, radius),
                                           new_position + math::Vec2f(radius, radius))))
            {
                position_valid = false;
                new_position.y = object.collider().GetBoundingBox().min_bound().y - radius - 2.0f; // Move down slightly and retry.
                break;
            }
        }

        if (position_valid) break;
    }

    switch (type)
    {
    case math::ShapeType::kAABB:
        {
            math::Vec2f half_size_vec = math::Vec2f(radius, radius);
            auto half_size_length = half_size_vec.Magnitude();
            math::AABB aabb(new_position, half_size_vec, half_size_length);
            CreateObject(i, aabb);
        }
        break;
    case math::ShapeType::kCircle:
        {
            math::Circle circle(new_position, radius);
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
    physics::Body body(physics::BodyType::Dynamic, circle.centre(), velocity, random::Range(0.1f, 0.5f));
    physics::Collider collider(circle, random::Range(0.5f, 0.9f), 0.1f, false);
    GameObject object(body, collider, circle.radius());

    objects_.push_back(object);
    RegisterObject(objects_[index]);
}

void FrictionSystem::CreateObject(size_t index, math::AABB& aabb)
{
    math::Vec2f velocity(0.0f, 0.0f);
    physics::Body body(physics::BodyType::Dynamic, aabb.GetCentre(), velocity, random::Range(0.1f, 0.5f));
    physics::Collider collider(aabb, random::Range(0.0f, 0.0f), 0.0f, false);
    GameObject object(body, collider, aabb.half_size_length());

    objects_.push_back(object);
    RegisterObject(objects_[index]);
}

void FrictionSystem::CreateGround()
{
    size_t i = objects_.size();

    math::AABB ground(math::Vec2f(360.0f, 650.0f), math::Vec2f(840.0f, 750.0f));

    math::Vec2f velocity(0.0f, 0.0f);
    physics::Body body(physics::BodyType::Static, ground.GetCentre(), velocity, 0.0f);
    physics::Collider collider(ground, 0.0f, 0.0f, false);
    GameObject object(body, collider, ground.half_size_length());


    objects_.push_back(object);
    RegisterObject(objects_[i]);
}

void FrictionSystem::DeleteObject(size_t index)
{
    if (index >= objects_.size()) return;

    GameObject& object = objects_[index];
    UnregisterObject(object);
    objects_.erase(objects_.begin() + index);
}


void FrictionSystem::RegisterObject(GameObject& object)
{
    collider_to_object_map_[&object.collider()] = &object;
}

void FrictionSystem::UnregisterObject(GameObject& object)
{
    collider_to_object_map_.erase(&object.collider());
}

void FrictionSystem::Update(float delta_time)
{
    UpdateShapes(delta_time);
    BroadPhase();
    NarrowPhase();
}

void FrictionSystem::UpdateShapes(float delta_time)
{
    for (auto& object : objects_)
    {
        auto& body = object.body();
        auto& collider = object.collider();

        body.ApplyGravity(kGravity_);

        body.Update(delta_time);

        auto position = body.position();

        //Update the collider's position
        collider.UpdatePosition(position);
    }
}

void FrictionSystem::SimplisticBroadPhase()
{
    std::unordered_map<GameObjectPair, bool> new_potential_pairs;

    // Loop through all objects
    for (size_t i = 0; i < objects_.size(); ++i)
    {
        auto& objectA = objects_[i];
        auto& colliderA = objectA.collider();

        // Get the AABB of the first collider
        auto rangeA = colliderA.GetBoundingBox();

        // Compare with all other objects
        for (size_t j = i + 1; j < objects_.size(); ++j)
        {
            auto& objectB = objects_[j];
            auto& colliderB = objectB.collider();

            // Get the AABB of the second collider
            auto rangeB = colliderB.GetBoundingBox();

            // Check for AABB overlap
            if (math::Intersect(rangeA, rangeB))
            {
                GameObjectPair pair{&objectA, &objectB};
                new_potential_pairs[pair] = true;
            }
        }
    }

    // Update the potential pairs for narrow phase to process
    potential_pairs_ = std::move(new_potential_pairs);
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
    std::unordered_set<GameObjectPair> newActivePairs;

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
            newActivePairs.insert(pair);

            // If this is a new collision Keep for triggers?
            if (active_pairs_.find(pair) == active_pairs_.end())
            {
                OnPairCollideStart(pair);
            }
            else
            {
                OnPairCollideStay(pair);
            }
        }
    }

    for (const auto& pair : active_pairs_)
    {
        if (newActivePairs.find(pair) == newActivePairs.end())
        {
            OnPairCollideEnd(pair);
        }
    }
    active_pairs_ = std::move(newActivePairs);
}

//Called on the first collision frame
void FrictionSystem::OnPairCollideStart(const GameObjectPair& pair)
{
    if(!pair.gameObjectA_ || !pair.gameObjectB_){return;}

    pair.gameObjectA_->AddCollision();
    pair.gameObjectB_->AddCollision();

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
void FrictionSystem::OnPairCollideStay(const GameObjectPair& pair)
{
    if(!pair.gameObjectA_ || !pair.gameObjectB_){return;}

    if (pair.gameObjectA_->collider().is_trigger() || pair.gameObjectB_->collider().is_trigger())
    {
        pair.gameObjectA_->OnTriggerStay();
        pair.gameObjectB_->OnTriggerStay();
    }
    else
    {
        //pair.gameObjectA_->OnCollisionStay();
        //pair.gameObjectB_->OnCollisionStay();
    }
}

void FrictionSystem::OnPairCollideEnd(const GameObjectPair& pair)
{
    if (!pair.gameObjectA_ || !pair.gameObjectB_) return;

    pair.gameObjectA_->SubCollision();
    pair.gameObjectB_->SubCollision();

    if (pair.gameObjectA_->collider().is_trigger() || pair.gameObjectB_->collider().is_trigger())
    {
        if (pair.gameObjectA_->collisions_count() <= 0)
        {
            pair.gameObjectA_->OnTriggerExit();
        }
        if (pair.gameObjectB_->collisions_count() <= 0)
        {
            pair.gameObjectB_->OnTriggerExit();
        }
    }
    else
    {

        if (pair.gameObjectA_->collisions_count() <= 0)
        {
            pair.gameObjectA_->OnCollisionExit();
        }
        if (pair.gameObjectB_->collisions_count() <= 0)
        {
            pair.gameObjectB_->OnCollisionExit();
        }
    }
}
