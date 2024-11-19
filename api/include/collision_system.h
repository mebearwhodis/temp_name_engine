#ifndef KUMA_ENGINE_API_COLLISION_SYSTEM_H_
#define KUMA_ENGINE_API_COLLISION_SYSTEM_H_

#include <unordered_map>

#include "game_object.h"
#include "quadtree.h"
#include "trigger_system.h"


class CollisionSystem
{
private:
    std::array<GameObject, kNumberOfShapes> objects_ = {};

    physics::Quadtree* quadtree_;

    std::unordered_map<GameObjectPair, bool> potential_pairs_;
    std::unordered_set<GameObjectPair> active_pairs_;

    std::unordered_map<physics::Collider*, GameObject*> collider_to_object_map_; //Mapping from Collider to GameObject

public:
    CollisionSystem();
    ~CollisionSystem() = default;

    void Initialize();
    void Update(float delta_time);
    void Clear();

    std::array<GameObject, kNumberOfShapes> objects() { return objects_; }
    [[nodiscard]] physics::Quadtree* quadtree() const { return quadtree_; }

    void CreateObject(size_t index, math::Circle& circle);
    void CreateObject(size_t index, math::AABB& aabb);
    void DeleteObject(size_t index);
    //void CreateObject(size_t index, math::Polygon& polygon);

    void RegisterObject(GameObject& object);
    void UnregisterObject(GameObject& object);

    void UpdateShapes(float delta_time);

    void SimplisticBroadPhase();
    void BroadPhase();
    void NarrowPhase();

    static void OnPairCollideStart(const GameObjectPair& pair);
    static void OnPairCollideStay(const GameObjectPair& pair);
    static void OnPairCollideEnd(const GameObjectPair& pair);
};
#endif // KUMA_ENGINE_API_COLLISION_SYSTEM_H_
