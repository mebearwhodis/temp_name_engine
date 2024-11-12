#ifndef FRICTION_SYSTEM_H
#define FRICTION_SYSTEM_H
#include "game_object.h"
#include "math/shape.h"
#include "physics/quadtree.h"

class FrictionSystem
{
private:
    std::vector<GameObject> objects_;

    physics::Quadtree* quadtree_;

    std::unordered_map<GameObjectPair, bool> potential_pairs_;
    std::unordered_map<GameObjectPair, bool> active_pairs_;

    std::unordered_map<physics::Collider*, GameObject*> collider_to_object_map_; //Mapping from Collider to GameObject
public:
    FrictionSystem();

    void CreateGround();

    std::vector<GameObject> objects() { return objects_; }
    [[nodiscard]] physics::Quadtree* quadtree() const { return quadtree_; }

    void SpawnShape(math::Vec2f pos, math::ShapeType type);
    void CreateObject(size_t index, math::Circle& circle);
    void CreateObject(size_t index, math::AABB& aabb);
    //void CreateObject(size_t index, math::Polygon& polygon);

    void RegisterObject(GameObject& object);
    void UnregisterObject(GameObject& object);

    void Update();
    void UpdateShapes();

    void BroadPhase();
    void NarrowPhase();

    static void OnPairCollide(const GameObjectPair& pair);
    static void OnPairCollideEnd(const GameObjectPair& pair);

};

#endif //FRICTION_SYSTEM_H
