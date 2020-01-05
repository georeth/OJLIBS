#include <gtest/gtest.h>
#include <random>
#include <ojlibs/mod_int.hpp>
#include <ojlibs/power.hpp>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

static const int MOD = 998244353;
typedef ojlibs::mod_int<MOD> mint;

TEST(BASIC, COMPILE) {
    mint a = 12;
    mint b = a * a;

    // mint c = 12 * b;
    mint c = mint(12) * b;
    mint d = power(c, 120);
}
