#include <gtest/gtest.h>
#include "math/four_vec3.h" // Include your FourVec3 header

namespace math {
    TEST(FourVec3Test, Addition) {
        std::array<Vec3<float>, 4> a_arr = { Vec3<float>{1.0f, 2.0f, 3.0f}, Vec3<float>{4.0f, 5.0f, 6.0f}, Vec3<float>{7.0f, 8.0f, 9.0f}, Vec3<float>{10.0f, 11.0f, 12.0f} };
        std::array<Vec3<float>, 4> b_arr = { Vec3<float>{13.0f, 14.0f, 15.0f}, Vec3<float>{16.0f, 17.0f, 18.0f}, Vec3<float>{19.0f, 20.0f, 21.0f}, Vec3<float>{22.0f, 23.0f, 24.0f} };

        FourVec3<float> a(a_arr);
        FourVec3<float> b(b_arr);
        FourVec3<float> result = a + b;

        EXPECT_EQ(result.x[0], 14.0f);
        EXPECT_EQ(result.y[0], 16.0f);
        EXPECT_EQ(result.z[0], 18.0f);
        // Repeat for other indices...
    }

    TEST(FourVec3Test, Subtraction) {
        std::array<Vec3<float>, 4> a_arr = { Vec3<float>{25.0f, 26.0f, 27.0f}, Vec3<float>{28.0f, 29.0f, 30.0f}, Vec3<float>{31.0f, 32.0f, 33.0f}, Vec3<float>{34.0f, 35.0f, 36.0f} };
        std::array<Vec3<float>, 4> b_arr = { Vec3<float>{10.0f, 11.0f, 12.0f}, Vec3<float>{13.0f, 14.0f, 15.0f}, Vec3<float>{16.0f, 17.0f, 18.0f}, Vec3<float>{19.0f, 20.0f, 21.0f} };

        FourVec3<float> a(a_arr);
        FourVec3<float> b(b_arr);
        FourVec3<float> result = a - b;

        EXPECT_EQ(result.x[0], 15.0f);
        EXPECT_EQ(result.y[0], 15.0f);
        EXPECT_EQ(result.z[0], 15.0f);
        // Repeat for other indices...
    }
}
