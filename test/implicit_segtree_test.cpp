#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#define LOCAL_JUDGE
#include <ojlibs/ds/implicit_segtree.hpp>
using namespace std;
using namespace ojlibs;

const int TEST_GROUP = 20;
const int TEST_SIZE = 2;
const int TEST_NUM = 1000;
std::mt19937 gen;
using uni = std::uniform_int_distribution<>;
uni dist(0, 1000);

TEST(BASIC, random_flavour_one) {
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        implicit_segtree<int, binary_plus<int>, true> seg(-TEST_SIZE, TEST_SIZE);
        vector<int> vec(TEST_SIZE * 2);

        dist(gen);

        // random initial values
        for (int i = 0; i < TEST_SIZE * 2; ++i) {
            seg.update_element(i - TEST_SIZE, dist(gen));
            vec[i] = seg.query_include(i - TEST_SIZE, i - TEST_SIZE);
        }

        for (int i = 0; i < TEST_NUM; ++i) {
            // first set an element by some random value
            int val = uni(-100, 100)(gen);
            int ele = uni(0, TEST_SIZE * 2 - 1)(gen);

            seg.update_element(ele - TEST_SIZE, val);
            vec[ele] = val;

            // then compare query_range with naive implementation
            int b = uni(0, TEST_SIZE * 2)(gen);
            int e = uni(b, TEST_SIZE * 2)(gen);

            if (b < e)
                ASSERT_EQ(seg.query_range(b - TEST_SIZE, e - TEST_SIZE),
                    accumulate(vec.begin() + b, vec.begin() + e, 0));
            else if (b == e)
                ASSERT_EQ(seg.query_range(b - TEST_SIZE, e - TEST_SIZE), 0);
            else throw 0;
        }
    }
}
TEST(BASIC, random_flavour_two) {
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        implicit_segtree<int, binary_plus<int>, false> seg(-TEST_SIZE, TEST_SIZE);
        vector<int> vec(TEST_SIZE);

        // random initial values
        for (int i = 0; i < TEST_SIZE * 2; ++i) {
            seg.increase_include(i - TEST_SIZE, i - TEST_SIZE, dist(gen));
            vec[i] = seg.query_element(i - TEST_SIZE);
        }

        for (int i = 0; i < TEST_NUM; ++i) {
            int b = uni(0, TEST_SIZE * 2)(gen);
            int e = uni(b, TEST_SIZE * 2)(gen);

            // first increase the range by some random value
            int inc = uni(-100, 100)(gen);
            seg.increase_range(b - TEST_SIZE, e - TEST_SIZE, inc);
            for (int i = b; i < e; ++i) vec[i] += inc;

            // then compare query_range with naive implementation
            int ele = uni(0, TEST_SIZE * 2 - 1)(gen);
            ASSERT_EQ(seg.query_element(ele - TEST_SIZE), vec[ele]);
        }
    }
}
