#ifndef OJLIBS_INC_SPARSE_MST_H_
#define OJLIBS_INC_SPARSE_MST_H_
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <ojlibs/sparse/graph.hpp>
#include <ojlibs/disjoint_set.hpp>

namespace ojlibs { // TO_BE_REMOVED

// Prim algorithm. by cut property : minimum edge in any cut is in some MST.
template<typename Sp, typename W>
std::vector<sp_entry> mst_prim(const graph<Sp, W> &g, int u) { // return vector of eid
    auto &sp = g.sp;
    auto &[wx] = g.vx;

    struct node_t {
        W w;
        sp_entry e;

        bool operator <(const node_t &that) const {
            return w > that.w;
        }
    };
    std::priority_queue<node_t> pq;
    std::vector<sp_entry> ret;
    std::vector<bool> in_tree(g.nvert());

    in_tree[u] = true;
    for (auto [v, eid] : sp.out(u))
        pq.push({wx[eid], {u, v, eid}});

    while (!pq.empty()) {
        auto [w, e] = pq.top();
        auto [from, to, eid] = e;
        pq.pop();

        if (in_tree[to]) continue;
        ret.push_back(e);
        in_tree[to] = true;

        for (auto [v, eid] : sp.out(to))
            pq.push({wx[eid], {to, v, eid}});
    }
    return ret;
}

// kruskal algorithm. prove by optimal property at each step.
template<typename Sp, typename W>
std::vector<sp_entry> mst_kruskal(graph<Sp, W> &g) {
    auto &sp = g.sp;
    auto &[wx] = g.vx;

    typedef std::pair<W, sp_entry> node_t;
    std::vector<node_t> edges;
    for (int u = 0; u < g.nvert() ; ++u)
        for (auto [v, eid] : sp.out(u))
            edges.push_back({wx[eid], {u, v, eid}});

    std::vector<sp_entry> tree;
    disjoint_set ds(g.nvert());
    std::sort(edges.begin(), edges.end(),
            [](const node_t &p1, const node_t &p2){
                return p1.first < p2.first;
            });

    for (auto [w, e] : edges) {
        if (ds.union_if(e.i, e.j)) {
            tree.push_back(e);
        }
    }
    return tree;
}


} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SPARSE_MST_H_ */
