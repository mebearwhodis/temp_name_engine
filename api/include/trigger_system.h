#ifndef TRIGGER_SYSTEM_H
#define TRIGGER_SYSTEM_H
#include <vector>

#include "game_object.h"
#include "physics/quadtree.h"


class TriggerSystem {
private:
    static constexpr size_t starting_number_of_shapes_ = 50;

    physics::Quadtree* quadtree_;
    std::vector<GameObject> objects_;

    std::unordered_map<physics::ColliderPair, bool> activePairs_;

public:
    TriggerSystem();

    std::vector<GameObject> objects() { return objects_; };

    void CreateObject(math::Circle& circle);
    void CreateObject(math::AABB& aabb);
    void CreateObject(math::Polygon& polygon);

    void Update();
    void UpdateShapes();

    void BroadPhase();
    void NarrowPhase();

    void OnTriggerEnter(physics::ColliderPair& pair);
    void OnTriggerExit(physics::ColliderPair& pair);
};



#endif //TRIGGER_SYSTEM_H
