#include <gtest/gtest.h>
#include <random>
#include <ojlibs/binary_search.hpp>
using namespace std;

TEST(BASIC, double) {
    double ex = M_PI;
    double d1 = ojlibs::dtff(0, 100, [=](double x) {
            return x < ex;
    });
    ASSERT_EQ(d1, ex);

    double d2 = ojlibs::dtft(0, 100, [=](double x) {
            return x <= ex;
    });
    ASSERT_EQ(d2, ex);

    double d3 = ojlibs::dftf(0, 100, [=](double x) {
            return x >= ex;
    });
    ASSERT_EQ(d3, std::nextafter(ex, ex - 1));

    double d4 = ojlibs::dftt(0, 100, [=](double x) {
            return x >= ex;
    });
    ASSERT_EQ(d4, ex);
}
TEST(BASIC, basic) {
    int n = 4;
    vector<int> inc = {10, 20, 30, 40};
    vector<int> dec = {40, 30, 20, 10};

    ASSERT_EQ(ojlibs::ftf(0, n, [&](int x) {
            return inc[x] > 21;
            }), 1);
    ASSERT_EQ(ojlibs::ftt(0, n, [&](int x) {
            return inc[x] > 21;
            }), 2);

    ASSERT_EQ(ojlibs::ftf(0, n, [&](int x) {
            return inc[x] >= 20;
            }), 0);
    ASSERT_EQ(ojlibs::ftt(0, n, [&](int x) {
            return inc[x] >= 20;
            }), 1);

    ASSERT_EQ(ojlibs::ftf(0, n, [&](int x) {
            return inc[x] > 0;
            }), 0);
    ASSERT_EQ(ojlibs::ftt(0, n, [&](int x) {
            return inc[x] > 100;
            }), 4);
}
