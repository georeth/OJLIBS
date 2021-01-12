#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <ojlibs/ds/segtree.hpp>
using namespace std;
using namespace ojlibs;

const int TEST_GROUP = 20;
const int TEST_SIZE = 10000;
const int TEST_NUM = 100;
std::mt19937 gen;
using uni = std::uniform_int_distribution<>;
uni dist(0, 1000);

TEST(BASIC, random_flavour_one) {
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        segtree<int, binary_max<int>, true> seg(TEST_SIZE);
        vector<int> vec(TEST_SIZE);

        // random initial values
        for (int i = 0; i < TEST_SIZE; ++i) {
            seg[i] = dist(gen);
            vec[i] = seg[i];

            ASSERT_EQ(vec[i], seg[i]);
        }
        seg.rebuild();

        for (int i = 0; i < TEST_NUM; ++i) {
            // first increase an element by some random value
            int inc = uni(-100, 100)(gen);
            int ele = uni(0, TEST_SIZE - 1)(gen);
            seg.update_element(ele, seg[ele] + inc);
            vec[ele] += inc;

            // then compare query_range with naive implementation
            int b = uni(0, TEST_SIZE)(gen);
            int e = uni(b, TEST_SIZE)(gen);
            if (b < e)
                ASSERT_EQ(seg.query_range(b, e),
                    *max_element(vec.begin() + b, vec.begin() + e));
            else if (b == e)
                ASSERT_EQ(seg.query_range(b, e), numeric_limits<int>::min());
            else throw 0;
        }
    }
}
TEST(BASIC, random_flavour_two) {
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        segtree<int, binary_plus<int>, false> seg(TEST_SIZE);
        vector<int> vec(TEST_SIZE);

        // random initial values
        for (int i = 0; i < TEST_SIZE; ++i) {
            seg[i] = dist(gen);
            vec[i] = seg[i];

            ASSERT_EQ(vec[i], seg[i]);
        }

        for (int i = 0; i < TEST_NUM; ++i) {
            int b = uni(0, TEST_SIZE)(gen);
            int e = uni(b, TEST_SIZE)(gen);

            // first increase the range by some random value
            int inc = uni(-100, 100)(gen);
            seg.increase_range(b, e, inc);
            for (int i = b; i < e; ++i) vec[i] += inc;

            // then compare query_range with naive implementation
            int ele = uni(0, TEST_SIZE - 1)(gen);
            ASSERT_EQ(seg.query_element(ele), vec[ele]);
        }

        // finally check flatten
        seg.flatten();
        for (int i = 0; i < TEST_SIZE; ++i)
            ASSERT_EQ(seg[i], vec[i]);
    }
}
