﻿#include "math/four_vec3.h"

namespace math
{
    // Operator+ with SIMD intrinsics
    template <>
    FourVec3f FourVec3<float>::operator+(const FourVec3<float>& other) const
    {
        FourVec3<float> result;
        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 x2 = _mm_loadu_ps(other.x.data());
        __m128 y2 = _mm_loadu_ps(other.y.data());
        __m128 z2 = _mm_loadu_ps(other.z.data());

        __m128 x_res = _mm_add_ps(x1, x2);
        __m128 y_res = _mm_add_ps(y1, y2);
        __m128 z_res = _mm_add_ps(z1, z2);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

    // Operator- with SIMD intrinsics
    template <>
    FourVec3f FourVec3<float>::operator-(const FourVec3<float>& other) const
    {
        FourVec3<float> result;
        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 x2 = _mm_loadu_ps(other.x.data());
        __m128 y2 = _mm_loadu_ps(other.y.data());
        __m128 z2 = _mm_loadu_ps(other.z.data());

        __m128 x_res = _mm_sub_ps(x1, x2);
        __m128 y_res = _mm_sub_ps(y1, y2);
        __m128 z_res = _mm_sub_ps(z1, z2);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

    // Negation operator
    template <>
    FourVec3f FourVec3<float>::operator-() const
    {
        FourVec3<float> result;
        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 neg_one = _mm_set1_ps(-1.0f);

        __m128 x_res = _mm_mul_ps(x1, neg_one);
        __m128 y_res = _mm_mul_ps(y1, neg_one);
        __m128 z_res = _mm_mul_ps(z1, neg_one);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

    // Scalar multiplication
    template <>
    FourVec3f FourVec3<float>::operator*(const float scalar) const
    {
        FourVec3<float> result;
        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 scalar_reg = _mm_set1_ps(scalar);
        __m128 x_res = _mm_mul_ps(x1, scalar_reg);
        __m128 y_res = _mm_mul_ps(y1, scalar_reg);
        __m128 z_res = _mm_mul_ps(z1, scalar_reg);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

    // Scalar division
    template <>
    FourVec3f FourVec3<float>::operator/(const float scalar) const
    {
        FourVec3<float> result;
        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 scalar_reg = _mm_set1_ps(scalar);
        __m128 x_res = _mm_div_ps(x1, scalar_reg);
        __m128 y_res = _mm_div_ps(y1, scalar_reg);
        __m128 z_res = _mm_div_ps(z1, scalar_reg);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

    // Dot product
    template <>
    std::array<float, 4> FourVec3<float>::Dot(const FourVec3f& other) const
    {
        std::array<float, 4> result;

        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 x2 = _mm_loadu_ps(other.x.data());
        __m128 y2 = _mm_loadu_ps(other.y.data());
        __m128 z2 = _mm_loadu_ps(other.z.data());

        __m128 x_mul = _mm_mul_ps(x1, x2);
        __m128 y_mul = _mm_mul_ps(y1, y2);
        __m128 z_mul = _mm_mul_ps(z1, z2);

        __m128 dot_res = _mm_add_ps(_mm_add_ps(x_mul, y_mul), z_mul);

        _mm_storeu_ps(result.data(), dot_res);

        return result;
    }

    // Square magnitude
    template <>
    std::array<float, 4> FourVec3<float>::SquareMagnitude() const
    {
        std::array<float, 4> result;

        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 x_sq = _mm_mul_ps(x1, x1);
        __m128 y_sq = _mm_mul_ps(y1, y1);
        __m128 z_sq = _mm_mul_ps(z1, z1);

        __m128 square_magnitude = _mm_add_ps(_mm_add_ps(x_sq, y_sq), z_sq);

        _mm_storeu_ps(result.data(), square_magnitude);

        return result;
    }

    // Magnitude
    template <>
    std::array<float, 4> FourVec3<float>::Magnitude() const
    {
        std::array<float, 4> squared_magnitude = SquareMagnitude();
        __m128 squared_magnitude_ps = _mm_loadu_ps(squared_magnitude.data());
        __m128 magnitude_ps = _mm_sqrt_ps(squared_magnitude_ps);

        std::array<float, 4> result;
        _mm_storeu_ps(result.data(), magnitude_ps);

        return result;
    }

    // Normalize
    template <>
    FourVec3f FourVec3<float>::Normalize() const
    {
        FourVec3<float> result;
        std::array<float, 4> magnitude = Magnitude();

        __m128 x1 = _mm_loadu_ps(x.data());
        __m128 y1 = _mm_loadu_ps(y.data());
        __m128 z1 = _mm_loadu_ps(z.data());

        __m128 magnitude_ps = _mm_loadu_ps(magnitude.data());

        __m128 x_res = _mm_div_ps(x1, magnitude_ps);
        __m128 y_res = _mm_div_ps(y1, magnitude_ps);
        __m128 z_res = _mm_div_ps(z1, magnitude_ps);

        _mm_storeu_ps(result.x.data(), x_res);
        _mm_storeu_ps(result.y.data(), y_res);
        _mm_storeu_ps(result.z.data(), z_res);

        return result;
    }

} // namespace math
