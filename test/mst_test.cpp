#include <gtest/gtest.h>
#include <random>
#include <set>
#include <utility>
#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/graph.hpp>
#include <ojlibs/sparse/mst.hpp>

using namespace std;
std::mt19937 gen;
using uni = uniform_int_distribution<>;
uniform_real_distribution<> dist_weight(0, 1000);

pair<ojlibs::lil, vector<double>> random_weighted_graph(int size_v, int size_e) {
    vector<int> roots;
    vector<double> wx;
    ojlibs::lil sp(size_v, size_v);
    ojlibs::graph g(sp, wx);

    set<pair<int, int>> visit;
    for (int i = 0; i < size_v; ++i)
        roots.push_back(i);

    // connected undirected graph
    while (roots.size() > 1) {
        int x = uni(0, (int)roots.size() - 1)(gen);
        swap(roots[0], roots[x]);
        x = 0;
        int y = uni(1, (int)roots.size() - 1)(gen);
        swap(roots[1], roots[y]);
        y = 1;

        double w = dist_weight(gen);
        g.add(roots[0], roots[1], w);
        g.add(roots[1], roots[0], w);
        visit.insert(make_pair(roots[0], roots[1]));
        visit.insert(make_pair(roots[1], roots[0]));

        swap(roots[0], roots.back());
        roots.pop_back();
        --size_e;
    }

    uni dist_v(0, size_v - 1);
    while (size_e-- > 0) {
        int x = dist_v(gen);
        int y = dist_v(gen);
        if (visit.count(make_pair(x, y))) continue;
        visit.insert(make_pair(x, y));
        visit.insert(make_pair(y, x));

        double w = dist_weight(gen);
        g.add(x, y, w);
        g.add(y, x, w);
    }
    return {sp, wx};
}

double weight_sum(const vector<double> &wx, const vector<ojlibs::sp_entry> &tree) {
    double ans = 0;
    for (auto &ent : tree)
        ans += wx[ent.eid];;
    return ans;
}

TEST(BASIC, random) {
    static const int VSIZE_MIN = 1000;
    static const int VSIZE_MAX = 2000;
    static const int ESIZE_MIN = 1000;
    static const int ESIZE_MAX = 20000;

    for (int T = 0; T < 20; ++T) {
        auto [sp, wx] = random_weighted_graph(uni(VSIZE_MIN, VSIZE_MAX)(gen), uni(ESIZE_MIN, ESIZE_MAX)(gen));
        ojlibs::graph g(sp, wx);
        vector<pair<double, pair<int, int>>> graph_edges;
        for (int from = 0; from < g.nvert(); from ++) {
            for (auto [to, w] : g.out(from))
                graph_edges.push_back(make_pair(w, make_pair(from, to)));
        }

        vector<ojlibs::sp_entry> T1 = ojlibs::mst_prim(g, 0);
        vector<ojlibs::sp_entry> T2 = ojlibs::mst_kruskal(g);

        ASSERT_EQ(T1.size(), g.nvert() - 1);
        ASSERT_EQ(T2.size(), g.nvert() - 1);
        ASSERT_FLOAT_EQ(weight_sum(wx, T1), weight_sum(wx, T2));
    }
}
