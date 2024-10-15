#ifndef LIBS_MATHS_FOUR_VEC3_H_
#define LIBS_MATHS_FOUR_VEC3_H_

#include <array>
#include <emmintrin.h>
#include "vec3.h"

namespace math
{
    template <typename T>
    struct FourVec3
    {
        std::array<T, 4> x;
        std::array<T, 4> y;
        std::array<T, 4> z;

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
    };

    // Specialization for int
    template<>
    struct FourVec3<int>
    {
        std::array<int, 4> x;
        std::array<int, 4> y;
        std::array<int, 4> z;

        FourVec3() = default;

        explicit FourVec3(const std::array<int, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i];
                y[i] = vec[i];
                z[i] = vec[i];
            }
        }

        FourVec3<int> operator+(const FourVec3<int>& other) const
        {
            FourVec3<int> result;
            __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.y.data()));
            __m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(z.data()));
            __m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.z.data()));

            __m128i x_res = _mm_add_epi32(x1, x2);
            __m128i y_res = _mm_add_epi32(y1, y2);
            __m128i z_res = _mm_add_epi32(z1, z2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.z.data()), z_res);

            return result;
        }

        FourVec3<int> operator-(const FourVec3<int>& other) const
        {
            FourVec3<int> result;
            __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.y.data()));
            __m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(z.data()));
            __m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.z.data()));

            __m128i x_res = _mm_sub_epi32(x1, x2);
            __m128i y_res = _mm_sub_epi32(y1, y2);
            __m128i z_res = _mm_sub_epi32(z1, z2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.z.data()), z_res);

            return result;
        }
    };

    // Specialization for float
    template <>
    struct FourVec3<float>
    {
        std::array<float, 4> x;
        std::array<float, 4> y;
        std::array<float, 4> z;

        FourVec3() = default;

        explicit FourVec3(const std::array<Vec3<float>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
                z[i] = vec[i].z;
            }
        }

        FourVec3<float> operator+(const FourVec3<float>& other) const
        {
            FourVec3<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());
            __m128 z1 = _mm_loadu_ps(z.data());
            __m128 z2 = _mm_loadu_ps(other.z.data());

            __m128 x_res = _mm_add_ps(x1, x2);
            __m128 y_res = _mm_add_ps(y1, y2);
            __m128 z_res = _mm_add_ps(z1, z2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);
            _mm_storeu_ps(result.z.data(), z_res);

            return result;
        }

        FourVec3<float> operator-(const FourVec3<float>& other) const
        {
            FourVec3<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());
            __m128 z1 = _mm_loadu_ps(z.data());
            __m128 z2 = _mm_loadu_ps(other.z.data());

            __m128 x_res = _mm_sub_ps(x1, x2);
            __m128 y_res = _mm_sub_ps(y1, y2);
            __m128 z_res = _mm_sub_ps(z1, z2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);
            _mm_storeu_ps(result.z.data(), z_res);

            return result;
        }
    };
}

#endif // LIBS_MATHS_FOUR_VEC3_H_
