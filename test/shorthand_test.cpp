#include <iostream>
#include <gtest/gtest.h>
#include <ojlibs/util/shorthand.hpp>
using namespace std;

TEST(BASIC, range_test) {
    FOR0(i, 100) { }
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
