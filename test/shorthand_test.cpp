#include <iostream>
#include <gtest/gtest.h>
#include <ojlibs/shorthand.hpp>
using namespace std;

TEST(BASIC, range_test) {
    vector<int> vec;
    for (int i = 0; i < 100; ++i)
        vec.push_back(i);

    int_range<> r = range(100);
    vector<int> vrange(r.begin(), r.end());

    ASSERT_EQ(vec.size(), vrange.size());
    ASSERT_TRUE(equal(vec.begin(), vec.end(), vrange.begin()));
    ASSERT_TRUE(equal(vec.rbegin(), vec.rend(), rev(r).begin()));
}

TEST(BASIC, chmax) {
    int a = 3;
    chmax(a, 5);
    ASSERT_EQ(5, a);

    int b = 3;
    chmin(b, -1);
    ASSERT_EQ(b, -1);

    pr_debug("meow %s %s\n", "moo", "woof");
}
