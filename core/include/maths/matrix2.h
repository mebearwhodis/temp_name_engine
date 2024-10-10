#ifndef CORE_MATHS_MATRIX2_H_
#define CORE_MATHS_MATRIX2_H_

#include <array>
#include "vec2.h"

namespace core
{
    template<typename T>
    struct matrix2
    {
        std::array<Vec2<T>, 2> rows_;

        // Default constructor
        matrix2() = default;

        // Constructor with parameters
        matrix2(const Vec2<T>& row1, const Vec2<T>& row2) : rows_{{row1, row2}} {}

        // Constructors with initializer lists (values and vectors)
        //matrix2(std::initializer_list<T> values) : rows_{values} {}
        //matrix2(std::initializer_list<Vec2<T>> rows) : rows_{rows} {}

        // Access row by index
        Vec2<T>& operator[](int index) {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            throw std::out_of_range("Index out of range for matrix2");
        }

        const Vec2<T>& operator[](int index) const {
            if (index == 0) return rows_[0];
            if (index == 1) return rows_[1];
            throw std::out_of_range("Index out of range for matrix2");
        }

        // Addition
        constexpr matrix2 operator+(const matrix2& other) const
        {
            return matrix2(rows_[0] + other[0], rows_[1] + other[1]);
        }

        // Subtraction
        constexpr matrix2 operator-(const matrix2& other) const
        {
            return matrix2(rows_[0] - other[0], rows_[1] - other[1]);
        }

        // Multiplication by scalar
        constexpr matrix2 operator*(T scalar) const
        {
            return matrix2(rows_[0] * scalar, rows_[1] * scalar);
        }

        // Multiplication by vector
        constexpr Vec2<T> operator*(const Vec2<T>& vec) const
        {
            return Vec2<T>(rows_[0].x * vec.x + rows_[0].y * vec.y, rows_[1].x * vec.x + rows_[1].y * vec.y);
        }

        // Determinant 1 using the Laplace Method
        [[nodiscard]] constexpr T determinant() const
        {
            return rows_[0].x * rows_[1].y - rows_[0].y * rows_[1].x;
        }

        // Transpose of the Matrix
        [[nodiscard]] constexpr matrix2 transpose() const
        {
            return matrix2(Vec2<T>(rows_[0].x, rows_[1].x), Vec2<T>(rows_[0].y, rows_[1].y));
        }

        // Inverse
        [[nodiscard]] constexpr matrix2 inverse() const
        {
            T det = determinant();
            if (det == 0) throw std::runtime_error("Matrix is not invertible");
            return matrix2(Vec2<T>(rows_[1].y / det, -rows_[0].y / det), Vec2<T>(-rows_[1].x / det, rows_[0].x / det));
        }

        // Rotation Matrix
        [[nodiscard]] constexpr matrix2 rotation(T angle) const
        {
            T c = std::cos(angle);
            T s = std::sin(angle);
            return matrix2(Vec2<T>(c, -s), Vec2<T>(s, c));
        }
    };

    // Multiplication by scalar (outside the struct)
    template <typename T>
    constexpr matrix2<T> operator*(T scalar, const matrix2<T>& mat)
    {
        return mat * scalar;
    }

    using matrix2f = matrix2<float>;
    using matrix2i = matrix2<int>;
    using matrix2d = matrix2<double>;

} // namespace core

#endif // CORE_MATHS_MATRIX2_H_
        //Add matrices
        //Subtract matrices
        //Multiply. Matrix * Scalar
        //Multiply. Scalar * Matrix (not using 'friend', but putting another override outside the struct)
        //Multiply by Vec3
        //Determinant 1 using the Laplace Method (call the function of matrix2)
        //Determinant 2 using the Sarrus Method
        //Transpose of the Matrix
        //Inverse
        //Rotation Matrix