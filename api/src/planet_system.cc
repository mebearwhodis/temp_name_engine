#include "planet_system.h"

void PlanetSystem::CreatePlanet(math::Vec2f position, float radius)
{
    auto planet = Body(position, math::Vec2f::Zero(), planet_mass_);

    math::Vec2f u = star_.position() - planet.position();
    float r = u.Magnitude();

    math::Vec2f angular_velocity = std::sqrt(gravitational_constant_ * (star_.mass() / r)) * math::Vec2f(-u.y, u.x).Normalized();

    planet.set_position(position);
    planet.set_velocity(angular_velocity);

    planets_.push_back(planet);
}

void PlanetSystem::UpdatePlanets()
{
    for(std::size_t i = 0; i < planets_.size(); i++)
    {
        auto& planet = planets_[i];

        math::Vec2f u = star_.position() - planet.position();
        float r = u.Magnitude();
        //TODO add check if r = 0
        float force_magnitude = gravitational_constant_ * (planet.mass() * star_.mass() / (r * r));
        math::Vec2f force = force_magnitude * u.Normalized();

        planet.ApplyForce(force);
        planet.set_velocity(planet.velocity() + planet.acceleration());
        planet.set_position(planet.position() + planet.velocity());
        planet.ResetForce();
    }
}

PlanetSystem::PlanetSystem()
{
    star_ = Body(math::Vec2f(600, 400), math::Vec2f::Zero(), star_mass_);
    for (std::size_t i = 0; i < starting_number_of_planets_; i++)
    {
        float test_pos = (std::rand() % 500) + 100;
        CreatePlanet(math::Vec2f(test_pos, test_pos), 50);
    }
}
