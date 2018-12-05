#include <gtest/gtest.h>
#include <random>
#include <functional>
#include <unordered_map>
#include <ojlibs/hash.hpp>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;
ojlibs::ojlibs_hash oh;

TEST(BASIC, ctor) {
    ASSERT_EQ(oh(12345), std::hash<int>()(12345));
    oh(make_pair(123, 345));
    oh(make_pair(123, make_pair(234, 345)));

    unordered_map<int, int, ojlibs::ojlibs_hash> u1;
    unordered_map<pair<int, int>, int, ojlibs::ojlibs_hash> u2;
}
