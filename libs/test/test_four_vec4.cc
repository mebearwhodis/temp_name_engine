#include <gtest/gtest.h>
#include "math/four_vec4.h" // Include your FourVec4 header

namespace math {
    TEST(FourVec4Test, Addition) {
        std::array<Vec4<float>, 4> a_arr = { Vec4<float>{1.0f, 2.0f, 3.0f, 4.0f}, Vec4<float>{5.0f, 6.0f, 7.0f, 8.0f}, Vec4<float>{9.0f, 10.0f, 11.0f, 12.0f}, Vec4<float>{13.0f, 14.0f, 15.0f, 16.0f} };
        std::array<Vec4<float>, 4> b_arr = { Vec4<float>{17.0f, 18.0f, 19.0f, 20.0f}, Vec4<float>{21.0f, 22.0f, 23.0f, 24.0f}, Vec4<float>{25.0f, 26.0f, 27.0f, 28.0f}, Vec4<float>{29.0f, 30.0f, 31.0f, 32.0f} };

        FourVec4<float> a(a_arr);
        FourVec4<float> b(b_arr);
        FourVec4<float> result = a + b;

        EXPECT_EQ(result.x[0], 18.0f);
        EXPECT_EQ(result.y[0], 20.0f);
        EXPECT_EQ(result.z[0], 22.0f);
        EXPECT_EQ(result.w[0], 24.0f);
        // Repeat for other indices...
    }

    TEST(FourVec4Test, Subtraction) {
        std::array<Vec4<float>, 4> a_arr = { Vec4<float>{50.0f, 51.0f, 52.0f, 53.0f}, Vec4<float>{54.0f, 55.0f, 56.0f, 57.0f}, Vec4<float>{58.0f, 59.0f, 60.0f, 61.0f}, Vec4<float>{62.0f, 63.0f, 64.0f, 65.0f} };
        std::array<Vec4<float>, 4> b_arr = { Vec4<float>{10.0f, 11.0f, 12.0f, 13.0f}, Vec4<float>{14.0f, 15.0f, 16.0f, 17.0f}, Vec4<float>{18.0f, 19.0f, 20.0f, 21.0f}, Vec4<float>{22.0f, 23.0f, 24.0f, 25.0f} };

        FourVec4<float> a(a_arr);
        FourVec4<float> b(b_arr);
        FourVec4<float> result = a - b;

        EXPECT_EQ(result.x[0], 40.0f);
        EXPECT_EQ(result.y[0], 40.0f);
        EXPECT_EQ(result.z[0], 40.0f);
        EXPECT_EQ(result.w[0], 40.0f);
        // Repeat for other indices...
    }
}
