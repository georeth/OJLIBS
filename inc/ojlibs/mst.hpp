#ifndef OJLIBS_INC_MST_H_
#define OJLIBS_INC_MST_H_
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <ojlibs/attr.hpp>
#include <ojlibs/graph.hpp>
#include <ojlibs/disjoint_set.hpp>

namespace ojlibs { // TO_BE_REMOVED

// Prim algorithm. by cut property : minimum edge in any cut is in some MST.
template<typename EAttr, typename WAccessor = atom_accessor<typename EAttr::value_type>>
std::vector<typename graph<EAttr>::E> mst_prim(graph<EAttr> &g, int u) {
    using W = typename WAccessor::value_type;
    using E = typename graph<EAttr>::E;
    WAccessor acc;

    std::vector<E> tree;
    std::vector<bool> in_tree(g.size());
    typedef std::pair<W, E> node_t;
    struct compare {
        bool operator()(const node_t &n1, const node_t &n2) {
            return n1.first > n2.first;
        }
    };
    std::priority_queue<node_t, std::vector<node_t>, compare> pq;

    in_tree[u] = true;
    for (E &e : g.edges[u])
        pq.push(std::make_pair(acc(e), e));

    while (!pq.empty()) {
        W w; E e;
        std::tie(w, e) = pq.top();
        pq.pop();

        int from = e.from, to = e.to;
        if (in_tree[to]) continue;

        tree.push_back(e);
        in_tree[to] = true;
        for (auto &e : g.edges[to])
            pq.push(std::make_pair(acc(e), e));
    }
    return tree;
}

// kruskal algorithm. prove by optimal property at each step.
template<typename EAttr, typename WAccessor = atom_accessor<typename EAttr::value_type>>
std::vector<typename graph<EAttr>::E> mst_kruskal(graph<EAttr> &g) {
    using W = typename WAccessor::value_type;
    using E = typename graph<EAttr>::E;
    WAccessor acc;

    int max_v = g.size();
    typedef std::pair<W, E> pair_type;
    std::vector<std::pair<W, E>> edges;
    for (int i = 0; i < max_v; ++i)
        for (auto &e : g.edges[i])
            edges.push_back(std::make_pair(acc(e), e));

    std::vector<E> tree;
    disjoint_set ds(max_v);
    std::sort(edges.begin(), edges.end(),
            [](const pair_type &p1, const pair_type &p2){
                return p1.first < p2.first;
            });
    for (auto &we : edges) {
        E e = we.second;
        if (ds.union_if(e.from, e.to)) {
            tree.push_back(e);
        }
    }
    return tree;
}

    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MST_H_ */
