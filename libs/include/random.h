#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace random{

[[nodiscard]] inline int Range(const int min, const int max) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

[[nodiscard]] inline float Range(const float min, const float max) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

}
#endif //RANDOM_H
