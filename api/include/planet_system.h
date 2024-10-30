#ifndef PLANET_SYSTEM_H
#define PLANET_SYSTEM_H
#include <vector>

#include "game_object.h"
#include "physics/body.h"
#include "math/vec2.h"

class PlanetSystem
{
private:
    static constexpr float kGravitationConstant_ = 0.0667f;
    static constexpr std::size_t kStartingPlanetsCount_ = 20;

    float star_mass_ = 10.f;
    float planet_mass_ = 3.f;

    physics::Body star_;
    std::vector<GameObject> planets_{};

    bool spawner_ = false;


public:
    PlanetSystem();

    void CreatePlanet(math::Vec2f position, float radius, const SDL_Color color);
    void UpdatePlanets();
    void UpdatePlanetsSIMD();
    void SpawnPlanets(math::Vec2f position);

    std::vector<GameObject> planets() { return planets_; }

    void ToggleSpawner() { spawner_ = !spawner_; };
};

#endif //PLANET_SYSTEM_H