#include <gtest/gtest.h>
#include <random>
#include <ojlibs/counting.hpp>
using namespace std;
using namespace ojlibs;
using uni = std::uniform_int_distribution<>;
static const int MOD = 1000000007;
std::mt19937 gen;

static const int MAXN = 3005;
ojlibs::factorial_list F(MAXN + 1, MOD);
ojlibs::binomial_table<MAXN + 1, MOD> B;
uni n_dist(0, MAXN);

TEST(BASIC, compare) {

    static const int TEST_SIZE = 10000;

    for (int i = 0; i < TEST_SIZE; ++i) {
        int n = n_dist(gen);
        int r = n_dist(gen);
        ASSERT_EQ(F.C(n, r), B.C(n, r));
    }
}

int dummy;

TEST(TIME, F) {
    static const int TEST_SIZE = 10000000;
    for (int i = 0; i < TEST_SIZE; ++i) {
        int n = n_dist(gen);
        int r = n_dist(gen);
        dummy += F.C(n, r);
    }
}
TEST(TIME, B) {
    static const int TEST_SIZE = 10000000;
    for (int i = 0; i < TEST_SIZE; ++i) {
        int n = n_dist(gen);
        int r = n_dist(gen);
        dummy += B.C(n, r);
    }
}
