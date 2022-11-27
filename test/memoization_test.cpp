#include <cstdio>
#include <gtest/gtest.h>
#include <ojlibs/misc/memoization.hpp>
using namespace ojlibs::memoization;

struct memo_fibnacci : public map_memo_crtp<memo_fibnacci, int, int> {
    int func_impl(int n) {
        if (n < 2)
            return n;
        return (*this)(n - 1) + (*this)(n - 2);
    }
};
TEST(BASIC, fibnacci) {
    memo_fibnacci f;
    ASSERT_EQ(55, f(10));
    ASSERT_EQ(f(50), f(49) + f(48));

    map_memo<int, int> *pf = &f;
    ASSERT_EQ(55, (*pf)(10));
    ASSERT_EQ((*pf)(50), (*pf)(49) + (*pf)(48));
}
