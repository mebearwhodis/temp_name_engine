#ifndef KUMA_ENGINE_LIB_COMMON_COMMON_H_
#define KUMA_ENGINE_LIB_COMMON_COMMON_H_

namespace common
{
    constexpr static float Pi = 3.14159265358979323846f;
    constexpr static float Epsilon = 0.000001f;

    template <typename T>
    [[nodiscard]] constexpr bool Approx(T value, T target)
    {
        return abs(value - target) <= Epsilon;
    }

} // namespace math
#endif // KUMA_ENGINE_LIB_COMMON_COMMON_H_
