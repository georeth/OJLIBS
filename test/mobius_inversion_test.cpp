#include <gtest/gtest.h>
#include <ojlibs/inversion/mobius_inversion.hpp>
#include <ojlibs/deprecated/binary_operator.hpp>
#include <random>
using namespace std;

std::mt19937 gen;
using uni = std::uniform_int_distribution<>;
using Op = ojlibs::modular_plus<int, 1000000007>;

TEST(BASIC, mobius_function) {
    const int TEST_SIZE = 10000;
    vector<int> u = ojlibs::mobius_function(TEST_SIZE);
    vector<int> _1(TEST_SIZE + 1, 0);
    _1[1] = 1;
    vector<int> uu = ojlibs::mobius_inverse(_1);

    ASSERT_EQ(uu, u);
}
TEST(BASIC, random) {
    const int TEST_GROUP = 100;
    const int TEST_SIZE_MIN = 1000;
    const int TEST_SIZE_MAX = 3000;
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        int n = uni(TEST_SIZE_MIN, TEST_SIZE_MAX)(gen);
        vector<int> f(n + 1);
        for (int i = 1; i <= n; ++i)
            f[i] = uni(0, 100)(gen);

        vector<int> g = ojlibs::mobius_transform(f, Op());
        vector<int> ff = ojlibs::mobius_inverse(g, Op());

        ASSERT_EQ(f, ff);
    }
}
