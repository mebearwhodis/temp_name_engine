#ifndef PLANET_SYSTEM_H
#define PLANET_SYSTEM_H
#include <vector>
#include "body.h"
#include "math/vec2.h"

class PlanetSystem
{
private:
    static constexpr float gravitational_constant_ = 0.0667f;
    static constexpr float star_mass_ = 10.f;
    static constexpr float planet_mass_ = 3.f;
    static constexpr std::size_t starting_number_of_planets_ = 3;

    Body star_;
    std::vector<Body> planets_;


public:
    PlanetSystem();

    void CreatePlanet(math::Vec2f position, float radius);
    void UpdatePlanets();

    std::vector<Body> planets() { return planets_; };
};

#endif //PLANET_SYSTEM_H