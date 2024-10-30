#include "planet_system.h"

#include "random.h"
#include "math/four_vec2.h"

//TODO: Replace all the planet.body() (get the body at the start in a variable)
void PlanetSystem::CreatePlanet(const math::Vec2f position, const float radius, const SDL_Color color)
{
    auto body = physics::Body(position, math::Vec2f::Zero(), planet_mass_);

    math::Vec2f u = star_.position() - body.position();
    float r = u.Magnitude();

    math::Vec2f angular_velocity = std::sqrt(kGravitationConstant_ * (star_.mass() / r)) * math::Vec2f(-u.y, u.x).
        Normalized();

    body.set_position(position);
    body.set_velocity(angular_velocity);

    auto planet = GameObject(body, radius, color);
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
            float force_magnitude = kGravitationConstant_ * (planet.body().mass() * star_.mass() / (r * r));
            math::Vec2f force = force_magnitude * u.Normalized();
            planet.body().ApplyForce(force);
        }

        planet.body().set_velocity(planet.body().velocity() + planet.body().acceleration());
        planet.body().set_position(planet.position() + planet.body().velocity());
        planet.body().ResetForce();
    }
}

void PlanetSystem::UpdatePlanetsSIMD()
{
    const std::size_t simdSize = planets_.size() / 4 * 4;

    for (std::size_t i = 0; i < simdSize; i += 4)
    {
        //Load four planet positions and calculate vector u to the star for each
        math::FourVec2f planetPositions = {
            planets_[i].position(),
            planets_[i+1].position(),
            planets_[i+2].position(),
            planets_[i+3].position()
        };

        math::FourVec2f u = math::FourVec2f({
            star_.position(), star_.position(), star_.position(), star_.position()
        }) - planetPositions;

        //Calculate distance to the star and force magnitudes
        std::array<float, 4> distances = u.Magnitude();
        math::FourVec2f normalizedU = u.Normalize();

        std::array<float, 4> forceMagnitudes;
        for (int j = 0; j < 4; ++j) {
            if (distances[j] > 0) {
                forceMagnitudes[j] = kGravitationConstant_ * (planets_[i+j].body().mass() * star_.mass() / (distances[j] * distances[j]));
            } else {
                forceMagnitudes[j] = 0.0f; // Handle edge case for zero distance
            }
        }

        // Apply forces and update position/velocity for each planet
        math::FourVec2f forces = normalizedU * forceMagnitudes;
        for (int j = 0; j < 4; ++j) {
            planets_[i+j].body().ApplyForce(math::Vec2f(forces.x[j], forces.y[j]));
            planets_[i+j].body().set_velocity(planets_[i+j].body().velocity() + planets_[i+j].body().acceleration());
            planets_[i+j].body().set_position(planets_[i+j].position() + planets_[i+j].body().velocity());
            planets_[i+j].body().ResetForce();
        }
    }

    // Handle any remaining planets that weren't in a set of four
    for (std::size_t i = simdSize; i < planets_.size(); ++i) {
        math::Vec2f u = star_.position() - planets_[i].position();
        float r = u.Magnitude();
        if (r > 0) {
            float force_magnitude = kGravitationConstant_ * (planets_[i].body().mass() * star_.mass() / (r * r));
            math::Vec2f force = force_magnitude * u.Normalized();
            planets_[i].body().ApplyForce(force);
        }
        planets_[i].body().set_velocity(planets_[i].body().velocity() + planets_[i].body().acceleration());
        planets_[i].body().set_position(planets_[i].position() + planets_[i].body().velocity());
        planets_[i].body().ResetForce();
    }
}

void PlanetSystem::SpawnPlanets(math::Vec2f position)
{
    if(spawner_ == true)
    {
        float radius = random::Range(5.f, 20.f);
        uint8_t alpha = random::Range(10, 255);
        CreatePlanet(position, radius, SDL_Color{ 255, 13, 132, alpha });
    }
}


PlanetSystem::PlanetSystem()
{
    star_ = physics::Body(math::Vec2f(600, 400), math::Vec2f::Zero(), star_mass_);
    for (std::size_t i = 0; i < kStartingPlanetsCount_; i++)
    {
        const math::Vec2f position(random::Range(20.f, 1180.f), random::Range(20.f, 780.f));
        float radius = random::Range(5.f, 20.f);
        uint8_t alpha = random::Range(10, 255);

        CreatePlanet(position, radius, SDL_Color{ 255, 13, 132, alpha });
    }
}
