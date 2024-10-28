#include "planet_system.h"

#include "math/four_vec2.h"

void PlanetSystem::CreatePlanet(math::Vec2f position, float radius)
{
    auto planet = physics::Body(position, math::Vec2f::Zero(), planet_mass_);

    math::Vec2f u = star_.position() - planet.position();
    float r = u.Magnitude();

    math::Vec2f angular_velocity = std::sqrt(gravitational_constant_ * (star_.mass() / r)) * math::Vec2f(-u.y, u.x).
        Normalized();

    planet.set_position(position);
    planet.set_velocity(angular_velocity);

    planets_.push_back(planet);
}

void PlanetSystem::UpdatePlanets()
{
    for (std::size_t i = 0; i < planets_.size(); i++)
    {
        auto& planet = planets_[i];

        math::Vec2f u = star_.position() - planet.position();
        float r = u.Magnitude();

        if (r > 0)
        {
            float force_magnitude = gravitational_constant_ * (planet.mass() * star_.mass() / (r * r));
            math::Vec2f force = force_magnitude * u.Normalized();
            planet.ApplyForce(force);
        }

        planet.set_velocity(planet.velocity() + planet.acceleration());
        planet.set_position(planet.position() + planet.velocity());
        planet.ResetForce();
    }
}

void PlanetSystem::UpdatePlanetsSIMD()
{
    const std::size_t simdSize = planets_.size() / 4 * 4;

    for (std::size_t i = 0; i < simdSize; i += 4)
    {
        // Load four planet positions and calculate vector u to the star for each
        math::FourVec2f planetPositions = {
            planets_[i].position(),
            planets_[i+1].position(),
            planets_[i+2].position(),
            planets_[i+3].position()
        };

        math::FourVec2f u = math::FourVec2f({
            star_.position(), star_.position(), star_.position(), star_.position()
        }) - planetPositions;

        // Calculate distance to the star and force magnitudes
        std::array<float, 4> distances = u.Magnitude();
        math::FourVec2f normalizedU = u.Normalize();

        std::array<float, 4> forceMagnitudes;
        for (int j = 0; j < 4; ++j) {
            if (distances[j] > 0) {
                forceMagnitudes[j] = gravitational_constant_ * (planets_[i+j].mass() * star_.mass() / (distances[j] * distances[j]));
            } else {
                forceMagnitudes[j] = 0.0f; // Handle edge case for zero distance
            }
        }

        // Apply forces and update position/velocity for each planet
        math::FourVec2f forces = normalizedU * forceMagnitudes;
        for (int j = 0; j < 4; ++j) {
            planets_[i+j].ApplyForce(math::Vec2f(forces.x[j], forces.y[j]));
            planets_[i+j].set_velocity(planets_[i+j].velocity() + planets_[i+j].acceleration());
            planets_[i+j].set_position(planets_[i+j].position() + planets_[i+j].velocity());
            planets_[i+j].ResetForce();
        }
    }

    // Handle any remaining planets that weren't in a set of four
    for (std::size_t i = simdSize; i < planets_.size(); ++i) {
        math::Vec2f u = star_.position() - planets_[i].position();
        float r = u.Magnitude();
        if (r > 0) {
            float force_magnitude = gravitational_constant_ * (planets_[i].mass() * star_.mass() / (r * r));
            math::Vec2f force = force_magnitude * u.Normalized();
            planets_[i].ApplyForce(force);
        }
        planets_[i].set_velocity(planets_[i].velocity() + planets_[i].acceleration());
        planets_[i].set_position(planets_[i].position() + planets_[i].velocity());
        planets_[i].ResetForce();
    }
}



PlanetSystem::PlanetSystem()
{
    star_ = physics::Body(math::Vec2f(600, 400), math::Vec2f::Zero(), star_mass_);
    for (std::size_t i = 0; i < starting_number_of_planets_; i++)
    {
        float test_pos = (std::rand() % 500) + 100;
        CreatePlanet(math::Vec2f(test_pos, test_pos), 50);
    }
}
