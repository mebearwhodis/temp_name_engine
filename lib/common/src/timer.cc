#include "timer.h"

Timer::Timer()
{
    starting_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::Tick() noexcept
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    delta_time_ = currentTime - starting_time_;
    accumulated_time_ += delta_time_;
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

void Timer::SetFixedDeltaTime(float seconds) noexcept
{
    fixed_delta_time_ = std::chrono::duration<float>(seconds);
}

bool Timer::FixedDeltaTimeStep() noexcept
{
    if (accumulated_time_ >= fixed_delta_time_)
    {
        accumulated_time_ -= fixed_delta_time_;
        return true;
    }
    return false;
}

float Timer::FixedDeltaTime() const noexcept
{
    return fixed_delta_time_.count();
}
