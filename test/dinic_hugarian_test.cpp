#include <random>
#include <vector>
#include <iostream>

#include <ojlibs/sparse/graph.hpp>
#include <ojlibs/sparse/dinic.hpp>
#include <ojlibs/dense/hungarian.hpp>
#include <ojlibs/dense/hungarian_short.hpp>
#include <gtest/gtest.h>

using namespace std;
std::mt19937 gen;

struct MAX_MATCH : testing::Test {
    vector<vector<int>> cost;
    void reset_cost(int size) {
        cost.resize(size);
        std::uniform_int_distribution<> dist(0, 100000);
        for (int i = 0; i < size; ++i)
            cost[i].resize(size);
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                cost[i][j] = dist(gen);
    }
};

TEST(BASIC, dinic) {
    ojlibs::dinic_max_flow<int> g(10);
    g.add(0, 1, 10);
    g.add(1, 0, 0);

    g.add(0, 2, 10);
    g.add(2, 0, 0);

    g.add(2, 3, 5);
    g.add(3, 2, 0);

    g.add(3, 1, 2);
    g.add(1, 3, 0);
    ASSERT_EQ(g.max_flow(0, 1), 12);
}
static const int TEST_GROUP = 20;
TEST_F(MAX_MATCH, correctness) {
    static const int TEST_SIZE = 50;
    for (int nr_group = 0; nr_group < TEST_GROUP; ++nr_group) {
        reset_cost(TEST_SIZE);

        int ans = ojlibs::hungarian<int>(cost).answer();
        ASSERT_EQ(ans, ojlibs::hungarian_short<int>(cost).answer());
    }
}

TEST_F(MAX_MATCH, hungarian) {
    static const int TEST_SIZE = 200;
    for (int nr_group = 0; nr_group < TEST_GROUP; ++nr_group) {
        reset_cost(TEST_SIZE);
        (void)ojlibs::hungarian<int>(cost).answer();
    }
}

TEST_F(MAX_MATCH, hungarian_short) {
    static const int TEST_SIZE = 200;
    for (int nr_group = 0; nr_group < TEST_GROUP; ++nr_group) {
        reset_cost(TEST_SIZE);
        (void)ojlibs::hungarian_short<int>(cost).answer();
    }
}
