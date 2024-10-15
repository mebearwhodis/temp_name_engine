#include <gtest/gtest.h>
#include "math/four_vec2.h" // Include your FourVec2 header

namespace math {
    TEST(FourVec2Test, Addition) {
        std::array<Vec2<float>, 4> a_arr = { Vec2<float>{1.0f, 2.0f}, Vec2<float>{3.0f, 4.0f}, Vec2<float>{5.0f, 6.0f}, Vec2<float>{7.0f, 8.0f} };
        std::array<Vec2<float>, 4> b_arr = { Vec2<float>{9.0f, 10.0f}, Vec2<float>{11.0f, 12.0f}, Vec2<float>{13.0f, 14.0f}, Vec2<float>{15.0f, 16.0f} };

        FourVec2<float> a(a_arr);
        FourVec2<float> b(b_arr);
        FourVec2<float> result = a + b;

        EXPECT_EQ(result.x[0], 10.0f);
        EXPECT_EQ(result.x[1], 14.0f);
        EXPECT_EQ(result.x[2], 18.0f);
        EXPECT_EQ(result.x[3], 22.0f);

        EXPECT_EQ(result.y[0], 12.0f);
        EXPECT_EQ(result.y[1], 16.0f);
        EXPECT_EQ(result.y[2], 20.0f);
        EXPECT_EQ(result.y[3], 24.0f);
    }

    TEST(FourVec2Test, Subtraction) {
        std::array<Vec2<float>, 4> a_arr = { Vec2<float>{10.0f, 20.0f}, Vec2<float>{30.0f, 40.0f}, Vec2<float>{50.0f, 60.0f}, Vec2<float>{70.0f, 80.0f} };
        std::array<Vec2<float>, 4> b_arr = { Vec2<float>{1.0f, 2.0f}, Vec2<float>{3.0f, 4.0f}, Vec2<float>{5.0f, 6.0f}, Vec2<float>{7.0f, 8.0f} };

        FourVec2<float> a(a_arr);
        FourVec2<float> b(b_arr);
        FourVec2<float> result = a - b;

        EXPECT_EQ(result.x[0], 9.0f);
        EXPECT_EQ(result.x[1], 27.0f);
        EXPECT_EQ(result.x[2], 45.0f);
        EXPECT_EQ(result.x[3], 63.0f);

        EXPECT_EQ(result.y[0], 18.0f);
        EXPECT_EQ(result.y[1], 36.0f);
        EXPECT_EQ(result.y[2], 54.0f);
        EXPECT_EQ(result.y[3], 72.0f);
    }
}
