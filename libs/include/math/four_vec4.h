#ifndef LIBS_MATHS_FOUR_VEC4_H_
#define LIBS_MATHS_FOUR_VEC4_H_

#include <array>
#include <emmintrin.h>
#include "vec4.h"

namespace math
{
    template <typename T>
    struct FourVec4
    {
        std::array<T, 4> x;
        std::array<T, 4> y;
        std::array<T, 4> z;
        std::array<T, 4> w;

        FourVec4() = default;

        explicit constexpr FourVec4(const std::array<Vec4<T>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
                z[i] = vec[i].z;
                w[i] = vec[i].w;
            }
        }

        FourVec4<T> operator+(const FourVec4<T>& other) const;
        FourVec4<T> operator-(const FourVec4<T>& other) const;
    };

    // Specialization for int
    template<>
    struct FourVec4<int>
    {
        std::array<int, 4> x;
        std::array<int, 4> y;
        std::array<int, 4> z;
        std::array<int, 4> w;

        FourVec4() = default;

        explicit FourVec4(const std::array<int, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i];
                y[i] = vec[i];
                z[i] = vec[i];
                w[i] = vec[i];
            }
        }

        FourVec4<int> operator+(const FourVec4<int>& other) const
        {
            FourVec4<int> result;
            __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.y.data()));
            __m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(z.data()));
            __m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.z.data()));
            __m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(w.data()));
            __m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.w.data()));

            __m128i x_res = _mm_add_epi32(x1, x2);
            __m128i y_res = _mm_add_epi32(y1, y2);
            __m128i z_res = _mm_add_epi32(z1, z2);
            __m128i w_res = _mm_add_epi32(w1, w2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.z.data()), z_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.w.data()), w_res);

            return result;
        }

        FourVec4<int> operator-(const FourVec4<int>& other) const
        {
            FourVec4<int> result;
            __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(x.data()));
            __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.x.data()));
            __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(y.data()));
            __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.y.data()));
            __m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(z.data()));
            __m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.z.data()));
            __m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(w.data()));
            __m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(other.w.data()));

            __m128i x_res = _mm_sub_epi32(x1, x2);
            __m128i y_res = _mm_sub_epi32(y1, y2);
            __m128i z_res = _mm_sub_epi32(z1, z2);
            __m128i w_res = _mm_sub_epi32(w1, w2);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.x.data()), x_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.y.data()), y_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.z.data()), z_res);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(result.w.data()), w_res);

            return result;
        }
    };

    // Specialization for float
    template <>
    struct FourVec4<float>
    {
        std::array<float, 4> x;
        std::array<float, 4> y;
        std::array<float, 4> z;
        std::array<float, 4> w;

        FourVec4() = default;

        explicit FourVec4(const std::array<Vec4<float>, 4>& vec)
        {
            for (int i = 0; i < 4; i++)
            {
                x[i] = vec[i].x;
                y[i] = vec[i].y;
                z[i] = vec[i].z;
                w[i] = vec[i].w;
            }
        }

        FourVec4<float> operator+(const FourVec4<float>& other) const
        {
            FourVec4<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());
            __m128 z1 = _mm_loadu_ps(z.data());
            __m128 z2 = _mm_loadu_ps(other.z.data());
            __m128 w1 = _mm_loadu_ps(w.data());
            __m128 w2 = _mm_loadu_ps(other.w.data());

            __m128 x_res = _mm_add_ps(x1, x2);
            __m128 y_res = _mm_add_ps(y1, y2);
            __m128 z_res = _mm_add_ps(z1, z2);
            __m128 w_res = _mm_add_ps(w1, w2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);
            _mm_storeu_ps(result.z.data(), z_res);
            _mm_storeu_ps(result.w.data(), w_res);

            return result;
        }

        FourVec4<float> operator-(const FourVec4<float>& other) const
        {
            FourVec4<float> result;
            __m128 x1 = _mm_loadu_ps(x.data());
            __m128 x2 = _mm_loadu_ps(other.x.data());
            __m128 y1 = _mm_loadu_ps(y.data());
            __m128 y2 = _mm_loadu_ps(other.y.data());
            __m128 z1 = _mm_loadu_ps(z.data());
            __m128 z2 = _mm_loadu_ps(other.z.data());
            __m128 w1 = _mm_loadu_ps(w.data());
            __m128 w2 = _mm_loadu_ps(other.w.data());

            __m128 x_res = _mm_sub_ps(x1, x2);
            __m128 y_res = _mm_sub_ps(y1, y2);
            __m128 z_res = _mm_sub_ps(z1, z2);
            __m128 w_res = _mm_sub_ps(w1, w2);

            _mm_storeu_ps(result.x.data(), x_res);
            _mm_storeu_ps(result.y.data(), y_res);
            _mm_storeu_ps(result.z.data(), z_res);
            _mm_storeu_ps(result.w.data(), w_res);

            return result;
        }
    };
}

#endif // LIBS_MATHS_FOUR_VEC4_H_
