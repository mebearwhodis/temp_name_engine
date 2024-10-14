#ifndef LIBS_MATHS_QUATERNION_H_
#define LIBS_MATHS_QUATERNION_H_
#include "vec3.h"

namespace math
{
    template <typename T>
    struct Quaternion
    {
        // W is the real part, V is the imaginary part comprised of x, y, z
        // Default is the identity quaternion
        T W{1};
        Vec3<T> V{0,0,0};

        Quaternion() = default;

        Quaternion(T w, const Vec3<T>& v) : W{w}, V{v} {}
        Quaternion(T w, T x, T y, T z) : W{w}, V{x, y, z} {};

    };
} // namespace math


#endif //LIBS_MATHS_QUATERNION_H_
