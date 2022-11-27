#include <gtest/gtest.h>
#include <random>
#include <ojlibs/misc/two_sat.hpp>
#include <ojlibs/sparse/lil.hpp>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

void check_scc(ojlibs::lil &g, vector<bool> ans) {
    for (int u = 0; u < g.r; ++u)
        for (int v : g.outv(u)) {
            bool c1 = ans[u / 2] ^ (u % 2);
            bool c2 = ans[v / 2] ^ (v % 2);

            // c1 -> c2 = c2 || !c1
            ASSERT_TRUE(c2 || !c1);
        }
}

TEST(BASIC, simple) {
    ojlibs::two_sat g(2);
    int c1 = 0, c2 = 1;
    g.add(~c1, ~c1);
    g.add(c2, c2);

    auto [succ, value] = g.solve();
    ASSERT_TRUE(succ);
    ASSERT_TRUE(!value[0]);
    ASSERT_TRUE(value[1]);

    check_scc(g.g, value);
}

TEST(BASIC, simple2) {
    ojlibs::two_sat g(3);
    g.add(0, 1);
    g.add(~1, ~2);

    auto [succ, value] = g.solve();
    ASSERT_TRUE(succ);
    check_scc(g.g, value);
}

TEST(BASIC, fail) {
    ojlibs::two_sat g(3);
    g.add(~0, ~0);
    g.add(0, 2);
    g.add(~2, ~1);
    g.add(1, 0);

    auto [succ, value] = g.solve();
    ASSERT_FALSE(succ);
}

TEST(RANDOM, small) {
    constexpr int TEST_CNT = 30;
    constexpr int VERT_CNT = 10;
    for (int test = 0; test < TEST_CNT; ++test) {
        vector<bool> solution(VERT_CNT); // make sure solution exist
        for (int u = 0; u < VERT_CNT; ++u)
            solution[u] = uniform_int_distribution<int>(0, 1)(gen);

        vector<pair<int, int>> edges;
        for (int u = 0; u < VERT_CNT; ++u)
            for (int v = 0; v < VERT_CNT; ++v) {
                edges.push_back({u, v});
                edges.push_back({~u, v});
                edges.push_back({u, ~v});
            }

        // shuffle
        for (int i = 0; i < (int)edges.size(); ++i) {
            int j = uniform_int_distribution<int>(i, (int)edges.size() - 1)(gen);
            swap(edges[i], edges[j]);
        }

        ojlibs::two_sat g(VERT_CNT);

        for (int last_e = 0; last_e < (int)edges.size(); ++last_e) {
            auto [u, v] = edges[last_e];
            g.add(u, v);
            auto [valid, value] = g.solve();
            ASSERT_TRUE(valid);
            check_scc(g.g, value);
        }
    }
}
