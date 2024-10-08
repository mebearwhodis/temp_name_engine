#ifndef CORE_MATHS_VEC2_H_
#define CORE_MATHS_VEC2_H_
#include <cmath>
#include <stdexcept>

namespace core
{
    /**
     * \brief Vec2 is a mathematical object represented by two values of type T
     */
    template <typename T>
    struct Vec2
    {
        T x{}, y{};

        // Default constructor
        Vec2() = default; // Default constructor

        // Constructor with parameters
        Vec2(T x, T y) : x(x), y(y) {}

        //Addition
        constexpr Vec2 operator+(const Vec2& other) const
        {
            return Vec2{ x + other.x, y + other.y };
        }

        //Subtraction
        constexpr Vec2 operator-(const Vec2 other) const
        {
            return { x - other.x, y - other.y };
        }

        //Opposite
        constexpr Vec2 operator-() const
        {
            return { -x, -y };
        }

        //Multiplication by scalar
        constexpr Vec2 operator*(T scalar) const
        {
            return{ x * scalar, y * scalar };
        }

        //Division by scalar
        constexpr Vec2 operator/(T scalar) const
        {
            if (scalar == 0) return { 0,0 };
            return { x / scalar, y / scalar };
        }

        T& operator[](int index) {
            if (index == 0) return x;
            if (index == 1) return y;
            throw std::out_of_range("Index out of range for Vec2");
        }

        const T& operator[](int index) const {
            if (index == 0) return x;
            if (index == 1) return y;
            throw std::out_of_range("Index out of range for Vec2");
        }

        //Dot product
        static constexpr T Dot(Vec2 v1, Vec2 v2)
        {
            return v1.x * v2.x + v1.y * v2.y;
        }

        //First perpendicular vector
        [[nodiscard]] constexpr Vec2 Perpendicular() const
        {
            return{ -y, x };
        }

        //Second perpendicular vector
        [[nodiscard]] constexpr Vec2 Perpendicular2() const
        {
            return{ y, -x };
        }

        //Vector squarelength
        [[nodiscard]] constexpr T SquareMagnitude() const
        {
            return x * x + y * y;
        }

        //Vector length

        [[nodiscard]] constexpr T Magnitude() const
        requires std::is_floating_point_v<T>
        {
            return std::sqrt(SquareMagnitude());
        }

        /**
         * \brief Normalized vector
         * \return if the vector's length is zero, return zero
         */
        [[nodiscard]] constexpr Vec2 Normalized() const
        {
            float mag = Magnitude();
            if (mag == 0) return { 0, 0 };
            return { x / mag, y / mag };
        }

        //Interpolate between this Vec2 and other by t amount
        [[nodiscard]] constexpr Vec2 LERP(Vec2 other, T t) const
        {
            return { x + t * (other.x - x), y + t * (other.y - y) };
        }
    }; // struct Vec2

    //Operator overload for multiplication
    template <typename T>
    constexpr Vec2<T> operator*(T scalar, const Vec2<T>& vec)
    {
        return { scalar * vec.x, scalar * vec.y };
    }

    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;
    using Vec2d = Vec2<double>;

} // namespace core
#endif //CORE_MATHS_VEC2_H_
