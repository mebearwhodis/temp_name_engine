#ifndef KUMA_ENGINE_LIB_COMMON_RANDOM_H_
#define KUMA_ENGINE_LIB_COMMON_RANDOM_H_

#include <random>

namespace random
{
    template <typename T>
    [[nodiscard]] T Range(const T min_number, const T max_number)
    {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");

        std::mt19937 rng(std::random_device{}());
        if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<int> dist(min_number, max_number);
            return dist(rng);
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<float> dist(min_number, max_number);
            return dist(rng);
        }
        throw std::invalid_argument("Invalid type");
    }
}
#endif //KUMA_ENGINE_LIB_COMMON_RANDOM_H_
