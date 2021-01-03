#include <gtest/gtest.h>
#include <ojlibs/shorthand.hpp>

TEST(BASIC, ctor) {
    iss is("15 20 meow");
    int a, b; string s;
    is, a, b, s;
    ASSERT_EQ(a, 15);
    ASSERT_EQ(b, 20);
    ASSERT_EQ(s, "meow");

    oss os;
    os, a, b, s;
    os.println(a, b, s);
    ASSERT_EQ(os.get(), "1520meow15 20 meow\n");
}
