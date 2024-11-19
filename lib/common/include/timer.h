#ifndef KUMA_ENGINE_LIB_COMMON_TIMER_H_
#define KUMA_ENGINE_LIB_COMMON_TIMER_H_

#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> starting_time_;
    std::chrono::duration<float> delta_time_ = std::chrono::duration<float>(0);
    std::chrono::duration<float> accumulated_time_ = std::chrono::duration<float>(0);
    std::chrono::duration<float> fixed_delta_time_ = std::chrono::duration<float>(1.0f / 60.0f);

public:
    Timer();
    void Tick() noexcept;
    [[nodiscard]] float DeltaTime() const noexcept;
    [[nodiscard]] float TotalTime() const noexcept;

    void SetFixedDeltaTime(float seconds) noexcept;
    [[nodiscard]] bool FixedDeltaTimeStep() noexcept;
    [[nodiscard]] float FixedDeltaTime() const noexcept;
};


#endif // KUMA_ENGINE_LIB_COMMON_TIMER_H_
