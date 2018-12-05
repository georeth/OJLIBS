#include <gtest/gtest.h>
#include <ojlibs/subset_inversion.hpp>
#include <ojlibs/binary_operator.hpp>
#include <vector>
#include <random>
using namespace std;
using uni = std::uniform_int_distribution<>;
using Op = ojlibs::modular_plus<int, 1000000007>;
std::mt19937 gen;
Op op;

TEST(RANDOM, subset) {
    static const int TEST_GROUP = 100;
    static const int K = 10;
    static const int M = 1 << K;
    for (int i = 0; i < TEST_GROUP; ++i) {
        vector<int> f(M);
        for (int x = 0; x < M; ++x)
            f[x] = uni(0, 1000)(gen);

        vector<int> g = ojlibs::subset_transform(f, K, Op());

        vector<int> gg(M);
        for (int x = 0; x < M; ++x)
            for (int y = 0; y < M; ++y) if ((y & x) == y)
                gg[x] = op(gg[x], f[y]);
        ASSERT_EQ(g, gg);

        vector<int> ff = ojlibs::subset_inverse(g, K, Op());
        ASSERT_EQ(f, ff);

        int x = uni(0, M - 1)(gen);
        EXPECT_EQ(f[x], ojlibs::subset_inverse_single(g, K, x, Op()));
    }
}
TEST(RANDOM, superset) {
    static const int TEST_GROUP = 100;
    static const int K = 10;
    static const int M = 1 << K;
    for (int i = 0; i < TEST_GROUP; ++i) {
        vector<int> f(M);
        for (int x = 0; x < M; ++x)
            f[x] = uni(0, 1000)(gen);

        vector<int> g = ojlibs::superset_transform(f, K, Op());

        vector<int> gg(M);
        for (int x = 0; x < M; ++x)
            for (int y = 0; y < M; ++y) if ((y & x) == x)
                gg[x] = op(gg[x], f[y]);

        ASSERT_EQ(g, gg);

        vector<int> ff = ojlibs::superset_inverse(g, K, Op());

        ASSERT_EQ(f, ff);

        int x = uni(0, M - 1)(gen);
        EXPECT_EQ(f[x], ojlibs::superset_inverse_single(g, K, x, Op()));
    }
}
