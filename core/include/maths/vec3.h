// #ifndef CORE_MATHS_VEC3_H_
// #define CORE_MATHS_VEC3_H_
// #include <cmath>
//
// namespace core
// {
//     /**
//      * \brief Vec3 is a mathematical object represented by three values of type T
//      */
//     template <typename T>
//     struct Vec3
//     {
//         T x = 0, y = 0, z = 0;
//
//         inline static const Vec3 Zero{0,0,0};
//         inline static const Vec3 One{1,1,1};
//
//
//
//         //Addition
//         constexpr Vec3 operator+(const Vec3& other) const
//         {
//             return Vec3{ x + other.x, y + other.y, z + other.z };
//         }
//
//         //Subtraction
//         constexpr Vec3 operator-(const Vec3& other) const
//         {
//             return { x - other.x, y - other.y, z - other.z };
//         }
//
//         //Opposite
//         constexpr Vec3 operator-() const
//         {
//             return { -x, -y, -z };
//         }
//
//         //Multiplication by scalar
//         constexpr Vec3 operator*(T scalar) const
//         {
//             return { x * scalar, y * scalar, z * scalar };
//         }
//
//         //Division by scalar
//         constexpr Vec3 operator/(T scalar) const
//         {
//             if (scalar == 0) return { 0, 0, 0 };
//             return { x / scalar, y / scalar, z / scalar };
//         }
//
//         //Dot product
//         static constexpr T Dot(const Vec3& v1, const Vec3& v2)
//         {
//             return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//         }
//
//         //Cross product
//         static constexpr Vec3 Cross(const Vec3& v1, const Vec3& v2)
//         {
//             return {
//                 v1.y * v2.z - v1.z * v2.y,
//                 v1.z * v2.x - v1.x * v2.z,
//                 v1.x * v2.y - v1.y * v2.x
//             };
//         }
//
//         //Vector square length
//         [[nodiscard]] constexpr T SquareMagnitude() const
//         {
//             return x * x + y * y + z * z;
//         }
//
//         //Vector length
//         //TODO add required
//         [[nodiscard]] T Magnitude() const
//         {
//             return std::sqrt(SquareMagnitude());
//         }
//
//         //Normalized vector
//         //TODO add required
//         [[nodiscard]] Vec3 Normalized() const
//         {
//             T mag = Magnitude();
//             if (mag == 0) return { 0, 0, 0 };
//             return { x / mag, y / mag, z / mag };
//         }
//
//         //Interpolate between this Vec3 and other by t amount
//         [[nodiscard]] constexpr Vec3 Lerp(const Vec3& other, T t) const
//         {
//             return { x + t * (other.x - x), y + t * (other.y - y), z + t * (other.z - z) };
//         }
//     }; // struct Vec3
//
//     //Operator overload for multiplication
//     template <typename T>
//     constexpr Vec3<T> operator*(T scalar, const Vec3<T>& vec)
//     {
//         return { scalar * vec.x, scalar * vec.y, scalar * vec.z };
//     }
//
// } // namespace core
// #endif // CORE_MATHS_VEC3_H_
