#ifndef CORE_MATHS_MATRIX4_H_
#define CORE_MATHS_MATRIX4_H_

#include <array>
#include "vec4.h"
#include "matrix3.h"  // For determinant using matrix3
#include <stdexcept>

namespace core
{
    template<typename T>
    struct matrix4
    {
        std::array<Vec4<T>, 4> rows;

        // Default constructor
        matrix4() = default;

        // Constructor with parameters
        matrix4(const Vec4<T>& row1, const Vec4<T>& row2, const Vec4<T>& row3, const Vec4<T>& row4) : rows{{row1, row2, row3, row4}} {}

        // Access row by index
        Vec4<T>& operator[](int index) {
            if (index == 0) return rows[0];
            if (index == 1) return rows[1];
            if (index == 2) return rows[2];
            if (index == 3) return rows[3];
            throw std::out_of_range("Index out of range for matrix4");
        }

        const Vec4<T>& operator[](int index) const {
            if (index == 0) return rows[0];
            if (index == 1) return rows[1];
            if (index == 2) return rows[2];
            if (index == 3) return rows[3];
            throw std::out_of_range("Index out of range for matrix4");
        }

        // Addition
        constexpr matrix4 operator+(const matrix4& other) const
        {
            return matrix4(rows[0] + other[0], rows[1] + other[1], rows[2] + other[2], rows[3] + other[3]);
        }

        // Subtraction
        constexpr matrix4 operator-(const matrix4& other) const
        {
            return matrix4(rows[0] - other[0], rows[1] - other[1], rows[2] - other[2], rows[3] - other[3]);
        }

        // Multiplication by scalar
        constexpr matrix4 operator*(T scalar) const
        {
            return matrix4(rows[0] * scalar, rows[1] * scalar, rows[2] * scalar, rows[3] * scalar);
        }

        // Multiplication by vector
        constexpr Vec4<T> operator*(const Vec4<T>& vec) const
        {
            return Vec4<T>(rows[0].x * vec.x + rows[0].y * vec.y + rows[0].z * vec.z + rows[0].w * vec.w,
                           rows[1].x * vec.x + rows[1].y * vec.y + rows[1].z * vec.z + rows[1].w * vec.w,
                           rows[2].x * vec.x + rows[2].y * vec.y + rows[2].z * vec.z + rows[2].w * vec.w,
                           rows[3].x * vec.x + rows[3].y * vec.y + rows[3].z * vec.z + rows[3].w * vec.w);
        }

        // Determinant 1 using the Laplace Method
        [[nodiscard]] constexpr T determinant() const
        {
            // Calculate the determinant using the Laplace expansion
            // along the first row
            return rows[0].x * core::matrix3<T>(Vec3<T>(rows[1].y, rows[1].z, rows[1].w), Vec3<T>(rows[2].y, rows[2].z, rows[2].w), Vec3<T>(rows[3].y, rows[3].z, rows[3].w)).determinant()
                   - rows[0].y * core::matrix3<T>(Vec3<T>(rows[1].x, rows[1].z, rows[1].w), Vec3<T>(rows[2].x, rows[2].z, rows[2].w), Vec3<T>(rows[3].x, rows[3].z, rows[3].w)).determinant()
                   + rows[0].z * core::matrix3<T>(Vec3<T>(rows[1].x, rows[1].y, rows[1].w), Vec3<T>(rows[2].x, rows[2].y, rows[2].w), Vec3<T>(rows[3].x, rows[3].y, rows[3].w)).determinant()
                   - rows[0].w * core::matrix3<T>(Vec3<T>(rows[1].x, rows[1].y, rows[1]. z), Vec3<T>(rows[2].x, rows[2].y, rows[2].z), Vec3<T>(rows[3].x, rows[3].y, rows[3].z)).determinant();
        }

        // Determinant 2 using the Sarrus Method
        [[nodiscard]] constexpr T determinantSarrus() const
        {
            // Calculate the determinant using the Sarrus rule
            return rows[0].x * rows[1].y * rows[2].z * rows[3].w
                   + rows[0].y * rows[1].z * rows[2].w * rows[3].x
                   + rows[0].z * rows[1].w * rows[2].x * rows[3].y
                   + rows[0].w * rows[1].x * rows[2].y * rows[3].z
                   - rows[0].x * rows[1].z * rows[2].w * rows[3].y
                   - rows[0].y * rows[1].w * rows[2].x * rows[3].z
                   - rows[0].z * rows[1].x * rows[2].y * rows[3].w
                   - rows[0].w * rows[1].y * rows[2].z * rows[3].x;
        }

        // Transpose of the Matrix
        [[nodiscard]] constexpr matrix4 transpose() const
        {
            return matrix4(Vec4<T>(rows[0].x, rows[1].x, rows[2].x, rows[3].x),
                           Vec4<T>(rows[0].y, rows[1].y, rows[2].y, rows[3].y),
                           Vec4<T>(rows[0].z, rows[1].z, rows[2].z, rows[3].z),
                           Vec4<T>(rows[0].w, rows[1].w, rows[2].w, rows[3].w));
        }
    };
}

#endif CORE_MATHS_MATRIX4_H_