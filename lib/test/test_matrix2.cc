#include <gtest/gtest.h>

#include <numbers>

#include "matrix2.h"

struct Matrix2Fixture : public ::testing::Test
{
};

TEST_F(Matrix2Fixture, MatrixAddition)
{
    math::matrix2<float> mat1{{1, 2}, {3, 4}};
    math::matrix2<float> mat2{{5, 6}, {7, 8}};
    auto result = mat1 + mat2;

    EXPECT_FLOAT_EQ(result[0].x, 6);
    EXPECT_FLOAT_EQ(result[0].y, 8);
    EXPECT_FLOAT_EQ(result[1].x, 10);
    EXPECT_FLOAT_EQ(result[1].y, 12);
}

TEST_F(Matrix2Fixture, MatrixSubtraction)
{
    math::matrix2<float> mat1{{5, 6}, {7, 8}};
    math::matrix2<float> mat2{{1, 2}, {3, 4}};
    auto result = mat1 - mat2;

    EXPECT_FLOAT_EQ(result[0].x, 4);
    EXPECT_FLOAT_EQ(result[0].y, 4);
    EXPECT_FLOAT_EQ(result[1].x, 4);
    EXPECT_FLOAT_EQ(result[1].y, 4);
}

TEST_F(Matrix2Fixture, ScalarMultiplication)
{
    math::matrix2<float> mat{{1, 2}, {3, 4}};
    auto result = mat * 2.0f;

    EXPECT_FLOAT_EQ(result[0].x, 2);
    EXPECT_FLOAT_EQ(result[0].y, 4);
    EXPECT_FLOAT_EQ(result[1].x, 6);
    EXPECT_FLOAT_EQ(result[1].y, 8);
}

TEST_F(Matrix2Fixture, MultiplyByVec2)
{
    math::matrix2<float> mat{{1, 2}, {3, 4}};
    math::Vec2<float> vec{2, 3};
    auto result = mat * vec;

    EXPECT_FLOAT_EQ(result.x, 8);
    EXPECT_FLOAT_EQ(result.y, 18);
}

TEST_F(Matrix2Fixture, DeterminantLaplace)
{
    math::matrix2<float> mat{{1, 2}, {3, 4}};
    auto det = mat.determinant();
    EXPECT_FLOAT_EQ(det, -2);
}

TEST_F(Matrix2Fixture, Transpose)
{
    math::matrix2<float> mat{{1, 2}, {3, 4}};
    auto transposed = mat.transpose();

    EXPECT_FLOAT_EQ(transposed[0].x, 1);
    EXPECT_FLOAT_EQ(transposed[0].y, 3);
    EXPECT_FLOAT_EQ(transposed[1].x, 2);
    EXPECT_FLOAT_EQ(transposed[1].y, 4);
}

TEST_F(Matrix2Fixture, Inverse)
{
    math::matrix2<float> mat{{1, 2}, {3, 4}};
    auto inverse = mat.inverse();
    EXPECT_FLOAT_EQ(inverse[0].x, -2);
    EXPECT_FLOAT_EQ(inverse[0].y, 1);
    EXPECT_FLOAT_EQ(inverse[1].x, 1.5);
    EXPECT_FLOAT_EQ(inverse[1].y, -0.5);
}

TEST_F(Matrix2Fixture, RotationMatrix)
{
    auto rotation = math::matrix2<float>().rotation(static_cast<float>(std::numbers::pi_v<float> / 2));
    EXPECT_NEAR(rotation[0].x, 0, 1e-6);
    EXPECT_NEAR(rotation[0].y, -1, 1e-6);
    EXPECT_NEAR(rotation[1].x, 1, 1e-6);
    EXPECT_NEAR(rotation[1].y, 0, 1e-6);
}
