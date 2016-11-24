#include <gtest/gtest.h>
#include <random>
#include <set>
#include <ojlibs/graph.hpp>
#include <ojlibs/mst.hpp>

using namespace std;
std::mt19937 gen;
using uni = uniform_int_distribution<>;
uniform_real_distribution<> dist_weight(0, 1000);

ojlibs::graph<double> random_weighted_graph(int size_v, int size_e) {
    vector<int> roots;
    ojlibs::graph<double> g(size_v, size_e * 2 + size_v * 2);
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
        g.add_edge(roots[0], roots[1], w);
        g.add_edge(roots[1], roots[0], w);
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
        g.add_edge(x, y, w);
        g.add_edge(y, x, w);
    }
    return g;
}

double weight_sum(ojlibs::graph<double> &g, vector<pair<int, int>> tree) {
    double ans = 0;
    map<pair<int, int>, double> W;
    for (int ie : ojlibs::range(g.edge_size())) {
        W[make_pair(g.edges[ie].from, g.edges[ie].to)] = g.edges[ie].info;
        W[make_pair(g.edges[ie].to, g.edges[ie].from)] = g.edges[ie].info;
    }

    for (auto xy : tree)
        ans += W[make_pair(xy.first, xy.second)];
    return ans;
}

TEST(BASIC, random) {
    static const int VSIZE_MIN = 1000;
    static const int VSIZE_MAX = 2000;
    static const int ESIZE_MIN = 1000;
    static const int ESIZE_MAX = 20000;

    for (int T = 0; T < 20; ++T) {
        ojlibs::graph<double> graph = random_weighted_graph(uni(VSIZE_MIN, VSIZE_MAX)(gen), uni(ESIZE_MIN, ESIZE_MAX)(gen));
        vector<pair<double, pair<int, int>>> graph_edges;
        for (int ie = 0; ie < graph.edge_size(); ie += 2)
            graph_edges.push_back(make_pair(graph.edges[ie].info, make_pair(graph.edges[ie].from, graph.edges[ie].to)));

        vector<pair<int, int>> T1 = ojlibs::mst_prim(graph, 0);
        vector<pair<int, int>> T2 = ojlibs::mst_kruskal(graph_edges, graph.vertex_size());

        ASSERT_EQ(T1.size(), graph.vertex_size() - 1);
        ASSERT_EQ(T2.size(), graph.vertex_size() - 1);
        ASSERT_FLOAT_EQ(weight_sum(graph, T1), weight_sum(graph, T2));
    }
}
