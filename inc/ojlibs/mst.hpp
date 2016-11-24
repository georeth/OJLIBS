#ifndef OJLIBS_INC_MST_H_
#define OJLIBS_INC_MST_H_
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <ojlibs/graph.hpp>
#include <ojlibs/disjoint_set.hpp>
#include <ojlibs/identity_visitor.hpp>

namespace ojlibs { // TO_BE_REMOVED

// prim algorithm. by cut property : minimum edge in any cut is in some MST.
template<typename EInfo, typename WVisitor = identity_visitor<EInfo>>
std::vector<std::pair<int, int>> mst_prim(graph<EInfo> &g, int u) {
    using W = decltype(WVisitor::get(EInfo()));

    std::vector<std::pair<int, int>> tree;
    std::vector<bool> in_tree(g.vertex_size());
    typedef std::pair<W, std::pair<int, int>> node_t;
    std::priority_queue<node_t, std::vector<node_t>, std::greater<node_t>> pq;

    in_tree[u] = true;
    for (auto &e : g.edge_list(u))
        pq.push(std::make_pair(WVisitor::get(e.info), std::make_pair(u, e.to)));

    while (!pq.empty()) {
        W w;
        int from, to;
        std::pair<int, int> ft;
        std::tie(w, ft) = pq.top();
        std::tie(from, to) = ft;
        pq.pop();
        if (in_tree[to]) continue;

        tree.push_back(std::make_pair(from, to));
        in_tree[to] = true;
        for (auto &e : g.edge_list(to))
            pq.push(std::make_pair(WVisitor::get(e.info), std::make_pair(to, e.to)));
    }
    return tree;
}

// kruskal algorithm. prove by optimal property at each step.
template<typename W>
std::vector<std::pair<int, int>> mst_kruskal(const std::vector<std::pair<W, std::pair<int, int>>> edges, int max_v) {
    std::vector<std::pair<int, int>> tree;
    disjoint_set ds(max_v);
    std::vector<std::pair<W, std::pair<int, int>>> E = edges;

    std::sort(E.begin(), E.end());
    for (auto &wxy : E) {
        W w;
        int x, y;
        std::pair<int, int> xy;
        std::tie(w, xy) = wxy;
        std::tie(x, y) = xy;
        if (ds.union_if(x, y)) {
            tree.push_back(xy);
        }
    }
    return tree;
}

    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MST_H_ */
