#ifndef TRIGGER_SYSTEM_H
#define TRIGGER_SYSTEM_H
#include <vector>

#include "game_object.h"


class TriggerSystem {
private:
    static constexpr size_t starting_number_of_shapes_ = 50;

    std::vector<GameObject> objects_;
public:
    TriggerSystem();

    std::vector<GameObject> objects() { return objects_; };

    void CreateObject(math::Circle& circle);
    void CreateObject(math::AABB& aabb);
    void CreateObject(math::Polygon& polygon);

    void Update();
    void UpdateShapes();

    void SolveBroadPhase();
    void SolveNarrowPhase();

    void OnTriggerEnter();
    void OnTriggerExit();
};



#endif //TRIGGER_SYSTEM_H
