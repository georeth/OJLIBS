#include <gtest/gtest.h>
#include <random>
#include <vector>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

struct move_test_t {
    bool by_copy = false; // propagate once copyed
    move_test_t() = default;
    move_test_t(const move_test_t &that) {
        by_copy = true;
    }
    move_test_t(move_test_t &&that) {
        by_copy = that.by_copy;
    }
};

struct info {
    move_test_t value;
};

move_test_t direct_return() {
    move_test_t t;
    return t;
}
info indirect_return() {
    move_test_t t;
    return { t };
}

info indirect_move_return() {
    move_test_t t;
    return { std::move(t) };
}

TEST(BASIC, t1) {
    ASSERT_EQ(direct_return().by_copy, false);
    ASSERT_EQ(indirect_return().value.by_copy, true);
    ASSERT_EQ(indirect_move_return().value.by_copy, false);
}
