#ifndef TRIGGER_SYSTEM_H
#define TRIGGER_SYSTEM_H
#include <vector>

#include "game_object.h"
#include "physics/quadtree.h"


static constexpr size_t kNumberOfShapes = 500;

class TriggerSystem {
private:

    std::array<GameObject, kNumberOfShapes> objects_ = {};

    physics::Quadtree* quadtree_;

    std::unordered_map<GameObjectPair, bool> potential_pairs_;
    std::unordered_map<GameObjectPair, bool> active_pairs_;

    std::unordered_map<physics::Collider*, GameObject*> collider_to_object_map_; //Mapping from Collider to GameObject

public:
    TriggerSystem();

    std::array<GameObject, kNumberOfShapes> objects() { return objects_; }
    [[nodiscard]] physics::Quadtree* quadtree() const { return quadtree_; }

    void CreateObject(size_t index, math::Circle& circle);
    //void CreateObject(math::AABB& aabb);
    //void CreateObject(math::Polygon& polygon);

    void RegisterObject(GameObject& object);
    void UnregisterObject(GameObject& object);

    void Update();
    void UpdateShapes();

    void SimplisticBroadPhase();
    void BroadPhase();
    void NarrowPhase();

    static void OnPairCollide(const GameObjectPair& pair);
    static void OnTriggerExit(const GameObjectPair& pair);
};



#endif //TRIGGER_SYSTEM_H
