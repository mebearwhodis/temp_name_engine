#ifndef TRIGGER_SYSTEM_H
#define TRIGGER_SYSTEM_H
#include <vector>

#include "game_object.h"
#include "physics/quadtree.h"


class TriggerSystem {
private:
    static constexpr size_t starting_number_of_shapes_ = 200;

    physics::Quadtree* quadtree_;
    std::vector<GameObject> objects_;

    std::unordered_map<GameObjectPair, bool> active_pairs_;

    std::unordered_map<physics::Collider*, GameObject*> collider_to_object_map_; // Mapping from Collider to GameObject

public:
    TriggerSystem();

    std::vector<GameObject> objects() { return objects_; };
    physics::Quadtree* quadtree() const { return quadtree_; };

    void CreateObject(math::Circle& circle);
    void CreateObject(math::AABB& aabb);
    void CreateObject(math::Polygon& polygon);

    void RegisterObject(GameObject& object);
    void UnregisterObject(GameObject& object);

    void Update();
    void UpdateShapes();

    void BroadPhase();
    void NarrowPhase();

    void OnTriggerEnter(const GameObjectPair& pair);
    void OnTriggerExit(const GameObjectPair& pair);
};



#endif //TRIGGER_SYSTEM_H
