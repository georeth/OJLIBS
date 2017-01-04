#include <ojlibs/geometry.hpp>
#include <gtest/gtest.h>
#include <random>

TEST(BASIC, ctor) {
    ojlibs::vec<3, double> dbl3;
    ojlibs::vec<3, int> int3;
}

TEST(BASIC, norm) {
    ojlibs::vec2<int> v1 = {3, 4};
    EXPECT_DOUBLE_EQ(v1.norm(), 5);
    EXPECT_EQ(v1.sq_norm(), 25);
}
