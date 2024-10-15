#ifndef LIBS_MATHS_FOUR_VEC2_H_
#define LIBS_MATHS_FOUR_VEC2_H_

#include <array>
#include <emmintrin.h>

#include "vec2.h"

namespace math
{
    template <typename T>
    struct FourVec2
    {
        std::array<T, 4> x;
        std::array<T, 4> y;

        explicit constexpr FourVec2(const std::array<Vec2<T>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
            }
        }

        FourVec2<T> operator+(const FourVec2<T>& other) const;
        FourVec2<T> operator-(const FourVec2<T>& other) const;
    };

    // Specialization for int
    template<>
    struct FourVec2<int>
    {
        std::array<int, 4> x;
        std::array<int, 4> y;

        explicit FourVec2(const std::array<int, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i];
                y[i] = vec[i];
            }
        }

        // Operator+ with SIMD intrinsics
        FourVec2<int> operator+(const FourVec2<int>& other) const
        {
            FourVec2<int> result;
            __m128i x1 = _mm_loadu_si128_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128_si128(reinterpret_cast<const __m128i*>(other.y.data()));

            __m128i x_res = _mm_add_epi32(x1, x2);
            __m128i y_res = _mm_add_epi32(y1, y2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);

            return result;
        }

        // Operator- with SIMD intrinsics
        FourVec2<int> operator-(const FourVec2<int>& other) const
        {
            FourVec2<int> result;
            __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.y.data()));

            __m128i x_res = _mm_sub_epi32(x1, x2);
            __m128i y_res = _mm_sub_epi32(y1, y2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);

            return result;
        }
    };

    // Specialization for float
    template <>
    struct FourVec2<float>
    {
        std::array<float, 4> x;
        std::array<float, 4> y;

        explicit FourVec2(const std::array<Vec2<float>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
            }
        }

        // Operator+ with SIMD intrinsics
        FourVec2<float> operator+(const FourVec2<float>& other) const
        {
            FourVec2<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());

            __m128 x_res = _mm_add_ps(x1, x2);
            __m128 y_res = _mm_add_ps(y1, y2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);

            return result;
        }

        // Operator- with SIMD intrinsics
        FourVec2<float> operator-(const FourVec2<float>& other) const
        {
            FourVec2<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());

            __m128 x_res = _mm_sub_ps(x1, x2);
            __m128 y_res = _mm_sub_ps(y1, y2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);

            return result;
        }
    };
}

#endif //LIBS_MATHS_FOUR_VEC2_H_