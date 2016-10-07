#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <ojlibs/segtree2d.hpp>
using namespace std;
using namespace ojlibs;


template <typename T>
struct max_traits {
    typedef std::true_type commutative;
    static void assoc_inplace_left(T &left, const T &right) {
        left = assoc(left, right);
    }
    static void assoc_inplace_right(const T &left, T &right) {
        right = assoc(left, right);
    }
    static T assoc_inplace(T &x, const T &y) {
        return x = max(x, y);
    }
    static T assoc(const T &x, const T &y) {
        return max(x, y);
    }
};

TEST(BASIC, random) {
    const int TEST_GROUP = 20;
    const int TEST_SIZE1 = 40, TEST_SIZE2=60;
    const int TEST_NUM = 100;
    std::mt19937 gen;

    using uni = std::uniform_int_distribution<>;
    uni dist(0, 1000);

    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        segtree2d<int, max_traits<int>> seg({TEST_SIZE1, TEST_SIZE2});
        vector<vector<int>> vec(TEST_SIZE1, vector<int>(TEST_SIZE2));

        for (int i = 0; i < TEST_SIZE1; ++i) {
        for (int j = 0; j < TEST_SIZE2; ++j) {
            seg[{i,j}] = dist(gen);
            vec[i][j] = seg[{i,j}];
        }
        }
        seg.rebuild();
        for (int i = 0; i < TEST_SIZE1; ++i) {
        for (int j = 0; j < TEST_SIZE2; ++j) {
            ASSERT_EQ(vec[i][j], seg.query_include({i,j},{i,j}));
        }
        }

        for (int i = 0; i < TEST_NUM; ++i) {
            int b1 = uni(0, TEST_SIZE1 - 1)(gen);
            int b2 = uni(0, TEST_SIZE2 - 1)(gen);
            int e1 = uni(b1, TEST_SIZE1 - 1)(gen);
            int e2 = uni(b2, TEST_SIZE2 - 1)(gen);

            int ans_seg = seg.query_include({b1, b2}, {e1, e2});
            int ans_vec = 0;
            for (int x = b1; x <= e1; ++x)
                for (int y = b2; y <= e2; ++y)
                    ans_vec = max(ans_vec, vec[x][y]);
            ASSERT_EQ(ans_seg, ans_vec);
        }
    }

}
