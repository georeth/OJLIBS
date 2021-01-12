#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include <ojlibs/sparse/csr.hpp>
#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/scc_kosaraju.hpp>
#include <ojlibs/sparse/scc_tarjan.hpp>

using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

vector<tuple<int, int, int>> coo = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 10},
    {1, 2, 2},
};

TEST(BASIC, all) {
    auto [csr, w] = ojlibs::to_csr(coo, 4, 4);

    {
        auto [scc_id, nscc] = ojlibs::scc_kosaraju(csr);
        printf("nscc = %d\n", nscc);
        for (int x : scc_id)
            printf("%d ", x);
        printf("\n");
    }

    {
        auto [scc_id, nscc] = ojlibs::scc_kosaraju(csr);
        printf("nscc = %d\n", nscc);
        for (int x : scc_id)
            printf("%d ", x);
        printf("\n");
    }
}

TEST(Random, s20) {
    constexpr int TEST_CNT = 40;
    constexpr int VERT_CNT = 20;
    for (int test = 0; test < TEST_CNT; ++test) {
        vector<pair<int, int>> edges;
        for (int u = 0; u < VERT_CNT; ++u)
            for (int v = 0; v < VERT_CNT; ++v) if (u != v)
                edges.push_back({u, v});
        for (int i = 0; i < (int)edges.size(); ++i) {
            int j = uniform_int_distribution<int>(i, (int)edges.size() - 1)(gen);
            swap(edges[i], edges[j]);
        }

        ojlibs::lil g(VERT_CNT, VERT_CNT);

        for (int last_e = 0; last_e < (int)edges.size(); ++last_e) {
            g.add(edges[last_e].first, edges[last_e].second);

            auto [scc1, nr_scc1] = ojlibs::scc_kosaraju(g);
            auto [scc2, nr_scc2] = ojlibs::scc_tarjan(g);

            ASSERT_EQ(nr_scc1, nr_scc2);

            for (int e = 0; e <= last_e; ++e) {
                auto [u, v] = edges[e];
                ASSERT_GE(scc1[u], scc1[u]);
                ASSERT_GE(scc2[u], scc2[u]);
            }

            vector<int> mapping(nr_scc1, -1);
            for (int i = 0; i < VERT_CNT; ++i) {
                if (mapping[scc1[i]] == -1)
                    mapping[scc1[i]] = scc2[i];
                ASSERT_EQ(mapping[scc1[i]], scc2[i]);
            }
        }
    }
}
