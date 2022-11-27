#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include <ojlibs/sparse/csr.hpp>
#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/graph.hpp>
#include <ojlibs/sparse/dijkstra.hpp>
#include <ojlibs/sparse/bellman.hpp>

using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

TEST(BASIC, ctor) {
    ojlibs::csr c{ .r = 3, .c = 3, .ix = {0, 2, 3, 3}, .jx = {0, 1, 2}};

    ojlibs::lil li(3, 3);
    li.add(0, 1);
    li.add(0, 2);
    li.add(1, 2);

    cout << to_string(c);
    cout << to_string(li);

    ojlibs::graph csr_g(c);
    for (int i = 0; i < csr_g.nvert(); ++i)
        for (auto [j] : csr_g.out(i))
            cout << "edge " << i << " " << j << '\n';

    cout << "\n";

    ojlibs::graph csr_li(li);
    for (int i = 0; i < csr_li.nvert(); ++i)
        for (auto [j] : csr_li.out(i))
            cout << "edge " << i << " " << j << '\n';

    cout << "\n";

    csr_li.add(2, 2);
    for (int i = 0; i < csr_li.nvert(); ++i)
        for (auto [j] : csr_li.out(i))
            cout << "edge " << i << " " << j << '\n';
}

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

TEST(BASIC, all) {
    auto [csr, w] = ojlibs::to_csr(coo, 4, 4);
    auto g0 = ojlibs::graph(csr);
    auto g1 = ojlibs::graph(csr, w);
    ASSERT_EQ(csr.r, 4);
    ASSERT_EQ(csr.c, 4);

    auto [dist, visit, pre] = ojlibs::dijkstra(g1, 1);
    auto [valid, dist_b, visit_b, pre_b] = ojlibs::bellman(g1, 1);
    vector<int> expect_dist{2, 0, 2, 12};
    vector<bool> expect_visit{true, true, true, true};
    vector<int> expect_pre{1, -1, 1, 0};

    ASSERT_EQ(dist, expect_dist);
    ASSERT_EQ(visit, expect_visit);
    ASSERT_EQ(pre, expect_pre);

    ASSERT_TRUE(valid);
    ASSERT_EQ(dist_b, expect_dist);
    ASSERT_EQ(visit_b, expect_visit);
    ASSERT_EQ(pre_b, expect_pre);
}

TEST(BASIC, transpose) {
    auto [csr, w] = ojlibs::to_csr(coo, 4, 4);
    auto t = ojlibs::transpose(csr);
    auto tt = ojlibs::transpose(t);

    ASSERT_EQ(csr.r, tt.r);
    ASSERT_EQ(csr.c, tt.c);
    ASSERT_EQ(csr.ix, tt.ix);
    ASSERT_EQ(csr.jx, tt.jx);
}
