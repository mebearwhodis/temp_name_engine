#ifndef KUMA_ENGINE_API_PLANET_SYSTEM_H_
#define KUMA_ENGINE_API_PLANET_SYSTEM_H_

#include <vector>

#include "body.h"
#include "game_object.h"
#include "vec2.h"

class PlanetSystem
{
private:
    static constexpr float kGravitationConstant_ = 0.06674f;
    static constexpr std::size_t kStartingPlanetsCount_ = 3;

    float star_mass_ = 10000.f;
    float planet_mass_ = 1000000.f;

    physics::Body star_;
    std::vector<GameObject> planets_{};

    bool is_spawner_active_ = false;

public:
    PlanetSystem();
    ~PlanetSystem() = default;

    void Initialize();
    void Update(float delta_time);
    void Clear();

    void CreatePlanet(math::Vec2f position, float radius, SDL_Color color);
    void UpdatePlanets(float delta_time);
    void UpdatePlanetsSIMD(float delta_time);
    void SpawnPlanets(math::Vec2f position);

    std::vector<GameObject> planets() { return planets_; }
    physics::Body* star() { return &star_; }

    void ToggleSpawner() { is_spawner_active_ = !is_spawner_active_; };
};

#endif // KUMA_ENGINE_API_PLANET_SYSTEM_H_
