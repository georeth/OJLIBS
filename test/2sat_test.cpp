#include <gtest/gtest.h>
#include <random>
#include <ojlibs/2sat.hpp>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

TEST(BASIC, simple) {
    ojlibs::graph<> g(4);
    int c1 = 0, c2 = 1;
    g.add(c1 * 2, c1 * 2 + 1);
    g.add(c2 * 2 + 1, c2 * 2);

    vector<bool> value;
    bool succ = ojlibs::find_2sat(g, value);
    ASSERT_TRUE(succ);
    ASSERT_TRUE(!value[0]);
    ASSERT_TRUE(value[1]);
}
