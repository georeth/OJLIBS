#include <gtest/gtest.h>
#include <string>
#include <ojlibs/string/kmp.hpp>
#include <ojlibs/string/z_func.hpp>
using namespace std;

using namespace ojlibs;

TEST(Basic, manual) {
    string s = "abcabdabcae";
    vector<int> p_expected = {0, 0, 0, 1, 2, 0, 1, 2, 3, 4, 0};
    vector<int> z_expected = {0, 0, 0, 2, 0, 0, 4, 0, 0, 1, 0};
    ASSERT_EQ(prefix_func(begin(s), end(s)), p_expected);
    ASSERT_EQ(z_func(begin(s), end(s)), z_expected);
}
