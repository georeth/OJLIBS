#include <vector>
#include <random>
#include <gtest/gtest.h>
#include <ojlibs/ds/sparse_table.hpp>
#include <ojlibs/util/shorthand.hpp>

TEST(BASIC, basic) {
    const int TEST_GROUP = 10;
    const int TEST_QUERY = 500;

    for (int tg = 0; tg < TEST_GROUP; ++tg) {
        const int TEST_SIZE = 100;
        std::mt19937 gen;
        using uni = std::uniform_int_distribution<>;
        uni dist(0, 10000);

        sparse_table<int> st(TEST_SIZE);
        vector<int> arr(TEST_SIZE);
        for (int i = 0; i < TEST_SIZE; ++i) {
            st.data()[i] = dist(gen);
            arr[i] = st.data()[i];
        }
        st.rebuild();

        for (int tq = 0; tq < TEST_QUERY; ++tq) {
            int x1 = uni(0, TEST_SIZE - 1)(gen);
            int x2 = uni(0, TEST_SIZE - 1)(gen);

            int st_ans = st.query_range(x1, x2);
            int arr_ans = numeric_limits<int>::min();
            for (int x = x1; x < x2; ++x)
                chmax(arr_ans, arr[x]);
            ASSERT_EQ(st_ans, arr_ans);
        }
    }
}
