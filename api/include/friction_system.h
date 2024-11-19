﻿#ifndef KUMA_ENGINE_API_FRICTION_SYSTEM_H_
#define KUMA_ENGINE_API_FRICTION_SYSTEM_H_

#include <unordered_map>
#include <unordered_set>

#include "display.h"
#include "game_object.h"
#include "quadtree.h"
#include "shape.h"
#include "timer.h"

class FrictionSystem
{
private:
    std::vector<GameObject> objects_;

    physics::Quadtree* quadtree_ = nullptr;

    std::unordered_map<GameObjectPair, bool> potential_pairs_;
    std::unordered_set<GameObjectPair> active_pairs_;

    std::unordered_map<physics::Collider*, GameObject*> collider_to_object_map_; //Mapping from Collider to GameObject

    Timer* timer_ = nullptr;
    math::AABB frame_bounds_ = math::AABB(math::Vec2f(0, 0), math::Vec2f(kWindowWidth, kWindowHeight));

public:
    FrictionSystem() = default;
    ~FrictionSystem();

    void Initialize();
    void Clear();

    std::vector<GameObject> objects() { return objects_; }
    [[nodiscard]] physics::Quadtree* quadtree() const { return quadtree_; }

    void SpawnShape(math::Vec2f pos, math::ShapeType type);
    void CreateObject(size_t index, math::Circle& circle);
    void CreateObject(size_t index, math::AABB& aabb);
    //void CreateObject(size_t index, math::Polygon& polygon);
    void CreateGround();
    void DeleteObject(size_t index);
    void RemoveOutOfBoundsObjects();

    void RegisterObject(GameObject& object);
    void UnregisterObject(GameObject& object);

    void Update(float delta_time);
    void UpdateShapes(float delta_time);

    void SimplisticBroadPhase();
    void BroadPhase();
    void NarrowPhase();

    static void OnPairCollideStart(const GameObjectPair& pair);
    static void OnPairCollideStay(const GameObjectPair& pair);
    static void OnPairCollideEnd(const GameObjectPair& pair);
};

#endif //KUMA_ENGINE_API_FRICTION_SYSTEM_H_
