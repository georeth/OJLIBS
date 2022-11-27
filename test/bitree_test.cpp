#include <random>
#include <gtest/gtest.h>
#include <ojlibs/ds/bitree.hpp>

std::mt19937 gen;

int query_range(const std::vector<int> &vec, int b, int e) {
    int ans = 0;
    while (b < e) {
        ans += vec[b];
        ++b;
    }
    return ans;
}

TEST(BASIC, random) {
    const int GROUP_SIZE = 20;
    const int TEST_SIZE = 1000;
    const int QUERY_SIZE = 300;
    std::uniform_int_distribution<> dist(-1000, 1000);

    for (int g = 0; g < GROUP_SIZE; ++g) {
        ojlibs::bitree<int> bit(TEST_SIZE);
        std::vector<int> vec(TEST_SIZE);
        std::uniform_int_distribution<> r_dist(0, TEST_SIZE);

        for (int i = 0; i < TEST_SIZE; ++i) {
            int v = dist(gen);
            bit.increase_element(i, v);
            vec[i] += v;
        }

        for (int t = 0; t < TEST_SIZE; ++t) {
            int b = r_dist(gen);
            int e = r_dist(gen);
            if (b <= e) {
                ASSERT_EQ(query_range(vec, b, e), bit.query_range(e) - bit.query_range(b));
            }
            if (b < vec.size())
                ASSERT_EQ(vec[b], bit.query_element(b));
        }
    }
}
