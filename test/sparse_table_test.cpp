#include <vector>
#include <random>
#include <gtest/gtest.h>
#include <ojlibs/sparse_table.hpp>
#include <ojlibs/shorthand.hpp>
using namespace std;
using namespace ojlibs::data_structure;

TEST(BASIC, basic) {
    const int TEST_GROUP = 10;
    const int TEST_QUERY = 500;

    for (int tg : range(TEST_GROUP)) {
        const int TEST_SIZE1 = 100;
        const int TEST_SIZE2 = 200;
        std::mt19937 gen;
        using uni = std::uniform_int_distribution<>;
        uni dist(0, 10000);

        sparse_table<int> st(TEST_SIZE1, TEST_SIZE2);
        vector<vector<int>> arr(TEST_SIZE1, vector<int>(TEST_SIZE2));
        for (int i : range(TEST_SIZE1))
        for (int j : range(TEST_SIZE2)) {
            st.data()[i][j] = dist(gen);
            arr[i][j] = st.data()[i][j];
        }
        st.rebuild();

        for (int tq : range(TEST_QUERY)) {
            int x1 = uni(0, TEST_SIZE1 - 1)(gen);
            int x2 = uni(0, TEST_SIZE1 - 1)(gen);
            int y1 = uni(0, TEST_SIZE2 - 1)(gen);
            int y2 = uni(0, TEST_SIZE2 - 1)(gen);

            int st_ans = st.query_range(x1, y1, x2, y2);
            int arr_ans = 0;
            for (int x : range(x1, x2))
                for (int y : range(y1, y2))
                    chmax(arr_ans, arr[x][y]);
            ASSERT_EQ(st_ans, arr_ans);
        }
    }
}
