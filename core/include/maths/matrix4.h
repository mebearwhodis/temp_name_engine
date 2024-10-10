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
        std::array<Vec4<T>, 4> rows_;

        // Default constructor
        matrix4() = default;

        // Constructor with parameters
        matrix4(const Vec4<T>& row1, const Vec4<T>& row2, const Vec4<T>& row3, const Vec4<T>& row4) : rows_{{row1, row2, row3, row4}} {}

        // Access row by index
        Vec4<T>& operator[](int index) {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            if (index == 2) return rows_[2];
            if (index == 3) return rows_[3];
            throw std::out_of_range("Index out of range for matrix4");
        }

        const Vec4<T>& operator[](int index) const {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            if (index == 2) return rows_[2];
            if (index == 3) return rows_[3];
            throw std::out_of_range("Index out of range for matrix4");
        }

        // Addition
        constexpr matrix4 operator+(const matrix4& other) const
        {
            return matrix4(rows_[0] + other[0], rows_[1] + other[1], rows_[2] + other[2], rows_[3] + other[3]);
        }

        // Subtraction
        constexpr matrix4 operator-(const matrix4& other) const
        {
            return matrix4(rows_[0] - other[0], rows_[1] - other[1], rows_[2] - other[2], rows_[3] - other[3]);
        }

        // Multiplication by scalar
        constexpr matrix4 operator*(T scalar) const
        {
            return matrix4(rows_[0] * scalar, rows_[1] * scalar, rows_[2] * scalar, rows_[3] * scalar);
        }

        // Multiplication by vector
        constexpr Vec4<T> operator*(const Vec4<T>& vec) const
        {
            return Vec4<T>(rows_[0].x * vec.x + rows_[0].y * vec.y + rows_[0].z * vec.z + rows_[0].w * vec.w,
                           rows_[1].x * vec.x + rows_[1].y * vec.y + rows_[1].z * vec.z + rows_[1].w * vec.w,
                           rows_[2].x * vec.x + rows_[2].y * vec.y + rows_[2].z * vec.z + rows_[2].w * vec.w,
                           rows_[3].x * vec.x + rows_[3].y * vec.y + rows_[3].z * vec.z + rows_[3].w * vec.w);
        }

        // Determinant 1 using the Laplace Method
        [[nodiscard]] constexpr T determinant() const
        {
            // Calculate the determinant using the Laplace expansion
            // along the first row
            return rows_[0].x * core::matrix3<T>(Vec3<T>(rows_[1].y, rows_[1].z, rows_[1].w), Vec3<T>(rows_[2].y, rows_[2].z, rows_[2].w), Vec3<T>(rows_[3].y, rows_[3].z, rows_[3].w)).determinant()
                   - rows_[0].y * core::matrix3<T>(Vec3<T>(rows_[1].x, rows_[1].z, rows_[1].w), Vec3<T>(rows_[2].x, rows_[2].z, rows_[2].w), Vec3<T>(rows_[3].x, rows_[3].z, rows_[3].w)).determinant()
                   + rows_[0].z * core::matrix3<T>(Vec3<T>(rows_[1].x, rows_[1].y, rows_[1].w), Vec3<T>(rows_[2].x, rows_[2].y, rows_[2].w), Vec3<T>(rows_[3].x, rows_[3].y, rows_[3].w)).determinant()
                   - rows_[0].w * core::matrix3<T>(Vec3<T>(rows_[1].x, rows_[1].y, rows_[1]. z), Vec3<T>(rows_[2].x, rows_[2].y, rows_[2].z), Vec3<T>(rows_[3].x, rows_[3].y, rows_[3].z)).determinant();
        }

        // Determinant 2 using the Sarrus Method
        [[nodiscard]] constexpr T determinantSarrus() const
        {
            // Calculate the determinant using the Sarrus rule
            return rows_[0].x * rows_[1].y * rows_[2].z * rows_[3].w
                   + rows_[0].y * rows_[1].z * rows_[2].w * rows_[3].x
                   + rows_[0].z * rows_[1].w * rows_[2].x * rows_[3].y
                   + rows_[0].w * rows_[1].x * rows_[2].y * rows_[3].z
                   - rows_[0].x * rows_[1].z * rows_[2].w * rows_[3].y
                   - rows_[0].y * rows_[1].w * rows_[2].x * rows_[3].z
                   - rows_[0].z * rows_[1].x * rows_[2].y * rows_[3].w
                   - rows_[0].w * rows_[1].y * rows_[2].z * rows_[3].x;
        }

        // Transpose of the Matrix
        [[nodiscard]] constexpr matrix4 transpose() const
        {
            return matrix4(Vec4<T>(rows_[0].x, rows_[1].x, rows_[2].x, rows_[3].x),
                           Vec4<T>(rows_[0].y, rows_[1].y, rows_[2].y, rows_[3].y),
                           Vec4<T>(rows_[0].z, rows_[1].z, rows_[2].z, rows_[3].z),
                           Vec4<T>(rows_[0].w, rows_[1].w, rows_[2].w, rows_[3].w));
        }
    };
}

#endif CORE_MATHS_MATRIX4_H_