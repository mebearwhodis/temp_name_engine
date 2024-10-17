#include "timer.h"

void Timer::Initialize() noexcept
{
    starting_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::Tick() noexcept
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    delta_time_ = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - starting_time_);

    starting_time_ = currentTime;
}

[[nodiscard]] float Timer::DeltaTime() const noexcept
{
    return delta_time_.count();
}

[[nodiscard]] float Timer::TotalTime() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<float>>(
            std::chrono::high_resolution_clock::now() - starting_time_).count();
}