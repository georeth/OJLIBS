#ifndef OJLIBS_INC_SPARSE_BELLMAN_H_
#define OJLIBS_INC_SPARSE_BELLMAN_H_

#include <queue>
#include <algorithm>
#include <ojlibs/util/util.hpp>
#include <ojlibs/sparse/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

// this code implements Bellman-Ford-Moore with no other optimizations.
// it can compute SSSP, or detect negative cycle when SSSP doesn't exist.
// worst-case time complexity is the same as Bellman-Ford, O(VE).
//
// Bellman-Ford do |V| iterations, and relax all edges in each iteration.
// with this optimization, we only relax out-edges of vertices whose distances are updated in previous iteration.
//
// there's no so-called SPFA algorithm, only Bellman-Ford variant with queue optimization (Bellman-Ford-Moore, 1959).
// the algorithm was re-invented as SPFA in 1994 with wrong time complexity anaysis (poor author thought it's O(|E|)).
// the name, SPFA, becomed a popular term in competitive programming community when a related NOI thesis released in 2009.
//
// we prefer B-F-M over vanilla B-F because:
// 1. our graph interface already group out-edges by source vertex
// 2. the optimization speeds up common cases, fall back to B-F in worst-case, and introduces little code complexity.
// 3. we need to check overflow in condition (dist[v] > dist[u] + w) in B-F
//    but B-F-M will visit this edge only after dist[u] is relaxed.
//
// SPFA may also refer to other optimizations (tricks) which claim to be faster in most cases.
// they do not guarantee to be as fast as B-F,
// and some of them even collapse to exponetial time on carefully generated graphs.

template <typename W>
struct bellman_info {
    bool valid;
    std::vector<W> dist;
    std::vector<bool> visit;
    std::vector<int> pre;
};

std::vector<int> find_cycle_bellman(const std::vector<int> &pre) {
    std::vector<int> initial(pre.size(), -1);
    for (int u = 0; u < pre.size(); ++u) {
        int v;
        for (v = u; v != -1 && initial[v] == -1; v = pre[v])
            initial[v] = u;
        if (v != -1 && initial[v] == u) {
            u = v; // now v, u is on cycle
            std::vector<int> ret;
            for (v = pre[u]; v != u; v = pre[v])
                ret.push_back(v);

            // pre is opposite with edge direction
            // make u at front of ret, and cycle ret is in original graph
            ret.push_back(u);
            std::reverse(ret.begin(), ret.end());
            return ret;
        }
    }
    return {};
}

template <typename W, typename WOut=W, typename Sp>
bellman_info<W> bellman(const graph<Sp, W> &g, int s) {
    int n = g.nvert();

    std::queue<int> q; // use fixed-size circular queue?

    bool valid = true;
    std::vector<int> visit_cnt(n); // increase when push and pop, odd means in q
    std::vector<WOut> dist(n, std::numeric_limits<WOut>::max());
    std::vector<int> pre(n, -1);

    dist[s] = 0;
    ++visit_cnt[s];
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (++visit_cnt[u] >= 2 * n) {
            // if there's a negative cycle, it will exist in pre[].
            // we know that follow pre[] from u will find a cycle, but we don't know whether u is on the cycle.
            // to avoid misuse, u is not included in return value, use find_cycle_bellman() instead.
            valid = false;
            break;
        }

        for (auto [v, w] : g.out(u)) {
            if (chmin(dist[v], dist[u] + w)) {
                pre[v] = u;
                if (~visit_cnt[v] & 1) { // not in queue now
                    ++visit_cnt[v];
                    q.push(v);
                }
            }
        }
    }
    std::vector<bool> visit(n);
    for (int i = 0; i < n; ++i)
        visit[i] = visit_cnt[i] > 0;
    return {valid, std::move(dist), std::move(visit), std::move(pre)};
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_BELLMAN_H_ */
