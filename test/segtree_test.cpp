#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <ojlibs/segtree.hpp>
using namespace std;
using namespace ojlibs::data_structure;


template <typename T>
struct max_traits : segtree_traits_crtp<T, max_traits<T>> {
    static T assoc(const T &x, const T &y) {
        return max(x, y);
    }
};

TEST(BASIC, random) {
    const int TEST_GROUP = 20;
    const int TEST_SIZE = 1000;
    const int TEST_NUM = 100;
    std::mt19937 gen;

    using uni = std::uniform_int_distribution<>;
    uni dist(0, 1000);

    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        segtree<int, max_traits<int>> seg(TEST_SIZE);
        vector<int> vec(TEST_SIZE);

        for (int i = 0; i < TEST_SIZE; ++i) {
            seg[i] = dist(gen);
            vec[i] = seg[i];

            ASSERT_EQ(vec[i], seg[i]);
        }
        seg.rebuild();
        for (int i = 0; i < TEST_NUM; ++i) {
            int b = uni(0, TEST_SIZE - 1)(gen);
            int e = uni(b, TEST_SIZE - 1)(gen);
            if (b < e) {
                ASSERT_EQ(seg.query_range(b, e),
                        *max_element(vec.begin() + b, vec.begin() + e));
            }
        }
        // seg.increase_element(10, 100);
    }
}
