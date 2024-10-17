#ifndef LIBS_MATHS_COMMON_
#define LIBS_MATHS_COMMON_

namespace math
{

    //TODO: faire un alias: using scalar = float
    constexpr static float Pi = 3.14159265358979323846f;
    constexpr static float Epsilon = 0.000001f;

    template<typename T>
    [[nodiscard]] constexpr bool Approx(T value, T target)
    {
        return abs(value - target) <= Epsilon;
    }

    //TODO in class angles:
    //    constexpr float ToRadian(float degree)
    //    {
    //      return degree*(pi/180.0f);
    //    }
}// namespace math
#endif // LIBS_MATHS_COMMON_