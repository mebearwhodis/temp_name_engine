﻿#ifndef KUMA_ENGINE_LIB_MATH_FOUR_VEC3_H_
#define KUMA_ENGINE_LIB_MATH_FOUR_VEC3_H_

#include <array>
#include <emmintrin.h>
#include <cassert>
#include "vec3.h"

namespace math
{
    template <typename T>
    struct FourVec3
    {
        std::array<T, 4> x{};
        std::array<T, 4> y{};
        std::array<T, 4> z{};

        FourVec3() = default;

        explicit constexpr FourVec3(const std::array<Vec3<T>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
                z[i] = vec[i].z;
            }
        }

        FourVec3<T> operator+(const FourVec3<T>& other) const;
        FourVec3<T> operator-(const FourVec3<T>& other) const;
        FourVec3<T> operator-() const; //Opposite
        FourVec3<T> operator*(const float scalar) const; //Multiply by scalar
        FourVec3<T> operator/(const float scalar) const; //Divide by scalar
        std::array<float, 4> Dot(const FourVec3<T>& other) const; //Dot
        std::array<float, 4> SquareMagnitude() const; //SquareMagnitude
        std::array<float, 4> Magnitude() const; //Magnitude/Sqrroot
        FourVec3<T> Normalize() const; //Normalize
    };

    using FourVec3i = FourVec3<int32_t>;
    using FourVec3f = FourVec3<float>;

    //Specialization for float
    template <>
    FourVec3f FourVec3<float>::operator+(const FourVec3f& other) const;
    template <>
    FourVec3f FourVec3<float>::operator-(const FourVec3f& other) const;
    template <>
    FourVec3f FourVec3<float>::operator-() const;
    template <>
    FourVec3f FourVec3<float>::operator*(const float scalar) const;
    template <>
    FourVec3f FourVec3<float>::operator/(const float scalar) const;
    template <>
    std::array<float, 4> FourVec3<float>::Dot(const FourVec3f& other) const;
    template <>
    std::array<float, 4> FourVec3<float>::SquareMagnitude() const;
    template <>
    std::array<float, 4> FourVec3<float>::Magnitude() const;
    template <>
    FourVec3f FourVec3<float>::Normalize() const;
}

#endif //KUMA_ENGINE_LIB_MATH_FOUR_VEC3_H_
