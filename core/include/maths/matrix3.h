#ifndef CORE_MATHS_MATRIX3_H_
#define CORE_MATHS_MATRIX3_H_

#include <array>
#include "vec3.h"
#include "matrix2.h"  // For determinant using matrix2
#include <stdexcept>

namespace core
{
    template<typename T>
    struct matrix3
    {
        std::array<Vec3<T>, 3> rows_;

        // Default constructor
        matrix3() = default;

        //TODO: constructor with initializer list

        // Constructor with parameters
        matrix3(const Vec3<T>& row1, const Vec3<T>& row2, const Vec3<T>& row3) : rows_{{row1, row2, row3}} {}

        // Access row by index
        Vec3<T>& operator[](int index) {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            if (index == 2) return rows_[2];
            throw std::out_of_range("Index out of range for matrix3");
        }

        const Vec3<T>& operator[](int index) const {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            if (index == 2) return rows_[2];
            throw std::out_of_range("Index out of range for matrix3");
        }

        // Addition
        constexpr matrix3 operator+(const matrix3& other) const
        {
            return matrix3(rows_[0] + other[0], rows_[1] + other[1], rows_[2] + other[2]);
        }

        // Subtraction
        constexpr matrix3 operator-(const matrix3& other) const
        {
            return matrix3(rows_[0] - other[0], rows_[1] - other[1], rows_[2] - other[2]);
        }

        // Multiplication by scalar
        constexpr matrix3 operator*(T scalar) const
        {
            return matrix3(rows_[0] * scalar, rows_[1] * scalar, rows_[2] * scalar);
        }

        // Multiplication by vector
        constexpr Vec3<T> operator*(const Vec3<T>& vec) const
        {
            return Vec3<T>(rows_[0].x * vec.x + rows_[0].y * vec.y + rows_[0].z * vec.z,
                           rows_[1].x * vec.x + rows_[1].y * vec.y + rows_[1].z * vec.z,
                           rows_[2].x * vec.x + rows_[2].y * vec.y + rows_[2].z * vec.z);
        }

        // Determinant 1 using the Laplace Method
        [[nodiscard]] constexpr T determinant() const
        {
            // Calculate the determinant using the Laplace expansion
            // along the first row
            return rows_[0].x * core::matrix2<T>(Vec2<T>(rows_[1].y, rows_[1].z), Vec2<T>(rows_[2].y, rows_[2].z)).determinant()
                   - rows_[0].y * core::matrix2<T>(Vec2<T>(rows_[1].x, rows_[1].z), Vec2<T>(rows_[2].x, rows_[2].z)).determinant()
                   + rows_[0].z * core::matrix2<T>(Vec2<T>(rows_[1].x, rows_[1].y), Vec2<T>(rows_[2].x, rows_[2].y)).determinant();
        }

        // Determinant 2 using the Sarrus Method
        [[nodiscard]] constexpr T determinantSarrus() const
        {
            // Calculate the determinant using the Sarrus rule
            return rows_[0].x * rows_[1].y * rows_[2].z
                   + rows_[0].y * rows_[1].z * rows_[2].x
                   + rows_[0].z * rows_[1].x * rows_[2].y
                   - rows_[0].x * rows_[1].z * rows_[2].y
                   - rows_[0].y * rows_[1].x * rows_[2].z
                   - rows_[0].z * rows_[1].y * rows_[2].x;
        }

        // Transpose of the Matrix
        [[nodiscard]] constexpr matrix3 transpose() const
        {
            return matrix3(Vec3<T>(rows_[0].x, rows_[1].x, rows_[2].x),
                           Vec3<T>(rows_[0].y, rows_[1].y, rows_[2].y),
                           Vec3<T>(rows_[0].z, rows_[1].z, rows_[2].z));
        }

        // Inverse
        [[nodiscard]] constexpr matrix3 inverse() const
        {
            T det = determinant();
            if ( det == 0) throw std::runtime_error("Matrix is singular");

            // Calculate the cofactor matrix
            matrix3 cofactor;
            cofactor[0].x = rows_[1].y * rows_[2].z - rows_[1].z * rows_[2].y;
            cofactor[0].y = rows_[1].z * rows_[2].x - rows_[1].x * rows_[2].z;
            cofactor[0].z = rows_[1].x * rows_[2].y - rows_[1].y * rows_[2].x;
            cofactor[1].x = rows_[0].z * rows_[2].y - rows_[0].y * rows_[2].z;
            cofactor[1].y = rows_[0].x * rows_[2].z - rows_[0].z * rows_[2].x;
            cofactor[1].z = rows_[0].y * rows_[2].x - rows_[0].x * rows_[2].y;
            cofactor[2].x = rows_[0].y * rows_[1].z - rows_[0].z * rows_[1].y;
            cofactor[2].y = rows_[0].z * rows_[1].x - rows_[0].x * rows_[1].z;
            cofactor[2].z = rows_[0].x * rows_[1].y - rows_[0].y * rows_[1].x;

            // Calculate the adjugate matrix
            matrix3 adjugate = cofactor.transpose();

            // Calculate the inverse matrix
            return adjugate * (1 / det);
        }

        // Rotation Matrix
        [[nodiscard]] constexpr matrix3 rotation(T angle) const
        {
            // Calculate the rotation matrix
            T cosAngle = std::cos(angle);
            T sinAngle = std::sin(angle);

            return matrix3(Vec3<T>(cosAngle, -sinAngle, 0),
                           Vec3<T>(sinAngle, cosAngle, 0),
                           Vec3<T>(0, 0, 1));
        }
    };

    // Multiplication by scalar (outside the struct)
    template<typename T>
    constexpr matrix3<T> operator*(T scalar, const matrix3<T>& mat)
    {
        return mat * scalar;
    }

} // namespace core

#endif // CORE_MATHS_MATRIX3_H_