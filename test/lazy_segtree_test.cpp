#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#define LOCAL_JUDGE
#include <ojlibs/shorthand.hpp> // XXX
#include <ojlibs/lazy_segtree.hpp>
using namespace std;
using namespace ojlibs;

std::mt19937 gen;
std::uniform_real_distribution<> dist(-10, 10);
using uni = std::uniform_int_distribution<>;

struct linear_op {
    double a, b;
    linear_op(double a = 1, double b = 0) : a(a), b(b) {}
    double operator()(double x, int len) const {
        return a * x + b * len;
    }
    operator bool() {
        return !(a == 1 && b == 0);
    }
    linear_op operator*(const linear_op &that) const {
        // a * (ta * x + tb) + b
        return linear_op(a * that.a, a * that.b + b);
    }
};

TEST(BASIC, random) {
    const int TEST_GROUP = 20;
    const int TEST_SIZE = 4;
    const int TEST_NUM = 4;
    typedef int flt_t;
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        lazy_segtree<flt_t, binary_plus<flt_t>, linear_op> seg(TEST_SIZE);
        vector<flt_t> vec(TEST_SIZE);

        // random initial values
        for (int i = 0; i < TEST_SIZE; ++i) {
            vec[i] = dist(gen);
            seg[i] = vec[i];
        }
        seg.rebuild();
        for (int i = 0; i < TEST_NUM; ++i) {
            int b = uni(0, TEST_SIZE)(gen);
            int e = uni(b, TEST_SIZE)(gen);

            flt_t ax = dist(gen);
            flt_t bx = dist(gen);

            // first increase the range by some random value
            seg.update_range(b, e, linear_op(ax, bx));
            for (int i = b; i < e; ++i) vec[i] = ax * vec[i] + bx;

            // then compare query_range with naive implementation
            double ans = 0;
            for (int i = b; i < e; ++i) ans += vec[i];
            ASSERT_FLOAT_EQ(ans, seg.query_range(b, e));
        }

        /*
        seg.flatten();
        for (int i = 0; i < TEST_SIZE; ++i)
            ASSERT_FLOAT_EQ(seg[i], vec[i]);
            */
    }
}

TEST(time, random) {
    const int TEST_GROUP = 20;
    const int TEST_SIZE = 10000;
    const int TEST_NUM = 10000;
    double ans = 0;
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        lazy_segtree<double, binary_plus<double>, linear_op> seg(TEST_SIZE);

        // random initial values
        for (int i = 0; i < TEST_SIZE; ++i) {
            seg[i] = dist(gen);
        }
        seg.rebuild();
        for (int i = 0; i < TEST_NUM; ++i) {
            int b = uni(0, TEST_SIZE)(gen);
            int e = uni(b, TEST_SIZE)(gen);

            double ax = exp(dist(gen) / 15);
            double bx = dist(gen);

            // first increase the range by some random value
            seg.update_range(b, e, linear_op(ax, bx));

            // then compare query_range with naive implementation
            ans += seg.query_range(b, e);
        }

        /*
        seg.flatten();
        for (int i = 0; i < TEST_SIZE; ++i)
            ASSERT_FLOAT_EQ(seg[i], vec[i]);
            */
    }
    cout << ans << endl;
}
