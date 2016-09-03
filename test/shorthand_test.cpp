#include <iostream>
#include <gtest/gtest.h>
#include <ojlibs/shorthand.hpp>
using namespace std;

TEST(BASIC, range) {
    for (int i : nrange(10))
        cout << i << endl;
    for (int i : range(1, -10))
        cout << i << endl;
    for (int i : range(3))
        cout << i << endl;
    for (int i : inrange(-2, 2))
        cout << i << endl;
}

TEST(BASIC, chmax) {
    int a = 3;
    chmax(a, 5);
    ASSERT_EQ(5, a);

    int b = 3;
    chmin(b, -1);
    ASSERT_EQ(b, -1);

    int c = 3;
    chmin_s(c, -1);
    ASSERT_EQ(c, 3);
}
