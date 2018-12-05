#include <ojlibs/test_type.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
using namespace std;
using namespace ojlibs;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

test_type tmp(123);

test_type f() {
    test_type v = test_type(100);
    printf("f: v = %d\n", v.v);
    if (tmp.v == 123) // if there is no `if', v ctor will be in-place. (c++ magic)
        return v;
    else
        return test_type(321);
}

test_type &g() {
    return tmp;
}

TEST(BASIC, t1) {
    vector<test_type> vec(5);
    for (auto &&k : vec) {
        printf("v = %d\n", k.v);
    }
    // auto && : the perfect forwarding
    auto &&t1 = f();
    printf("t1 : %d\n", t1.v);

    auto &&t2 = g();
    printf("t2 : %d\n", t2.v);
}
