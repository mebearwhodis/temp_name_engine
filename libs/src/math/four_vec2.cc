// #include "math/four_vec2.h"
//
// namespace math
// {
//     // Specialization for int constructor
//     FourVec2<int>::FourVec2(const std::array<Vec2<int>, 4>& vec)
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             x[i] = vec[i].x;
//             y[i] = vec[i].y;
//         }
//     }
//
//     // Specialization for operator+ for int
//     FourVec2<int> FourVec2<int>::operator+(const FourVec2<int>& other) const
//     {
//         FourVec2<int> result;
//         for (int i = 0; i < 4; i++)
//         {
//             result.x[i] = x[i] + other.x[i];
//             result.y[i] = y[i] + other.y[i];
//         }
//         return result;
//     }
//
//     // Specialization for operator- for int
//     FourVec2<int> FourVec2<int>::operator-(const FourVec2<int>& other) const
//     {
//         FourVec2<int> result;
//         for (int i = 0; i < 4; i++)
//         {
//             result.x[i] = x[i] - other.x[i];
//             result.y[i] = y[i] - other.y[i];
//         }
//         return result;
//     }
// }
