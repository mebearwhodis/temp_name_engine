#ifndef LIBS_MATHS_COMMON_
#define LIBS_MATHS_COMMON_

namespace math
{
    constexpr static float Pi = 3.14159265358979323846f;
    constexpr static float Epsilon = 0.000001f;

    template<typename T>
    [[nodiscard]] constexpr bool Approx(T value, T target)
    {
        return abs(value - target) <= Epsilon;
    }
}// namespace math
#endif // LIBS_MATHS_COMMON_