#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include <ojlibs/sparse/csr.hpp>
#include <ojlibs/sparse/dfs.hpp>

using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

vector<tuple<int, int, int>> coo = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 10},
    {1, 2, 2},

    {1, 0, 2},
    {2, 0, 3},
    {3, 0, 10},
    {2, 1, 2},
};

struct my_visitor : ojlibs::generic_dfs_visitor {
    void pre_edge(int u, int v, int e) {
        printf("pre_edge (%d, %d) %d\n", u, v, e);
    }
    void found_back(int u, int v, int e) {
        printf("back (%d, %d) %d\n", u, v, e);
    }
};

TEST(BASIC, all) {
    auto [csr, w] = ojlibs::to_csr(coo, 4, 4);

    my_visitor visitor;
    ojlibs::generic_dfs(csr, visitor, true);
    ojlibs::generic_dfs(csr, visitor, false);
}

