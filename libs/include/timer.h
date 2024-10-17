#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> starting_time_;
    std::chrono::duration<float> delta_time_ = std::chrono::duration<float>(0);

public:
    void Initialize() noexcept;
    void Tick() noexcept;
    [[nodiscard]] float DeltaTime() const noexcept;
    [[nodiscard]] float TotalTime() const noexcept;
};


#endif //TIMER_H
