#ifndef OJLIBS_INC_SPARSE_DINIC_H_
#define OJLIBS_INC_SPARSE_DINIC_H_

#include <queue>
#include <limits>
#include <ojlibs/sparse/graph.hpp>

// we provide a dinic(g, s, t) to find max flow.
// to use it, `graph<Sp, W> &g` must satisfy a requirement:
//    edge[eid] and edge[eid^1] have opposite direction
// (use lil as Sp, and add these two edges together)
// to avoid overflow, sum of capacity of these two edge <= W::max()
//
// when the dinic() finished, g is modified to resulting residual graph.
// you can get min-cut by dfs g, reconstruct the flow by diff g with original g.
//
// there are tricks in building graph, and the algorithm may run multiple times,
// so we only provide raw interface.

namespace ojlibs { // TO_BE_REMOVED

// time: O(V^2 E)
// unit capacity: O(min(V^(2/3), E^(1/2)) E)
// biparti-mathcing: O(sqrt(V) E)
template <typename WOut, typename Sp, typename W>
struct dinic_ctx {
    const Sp &sp;
    std::vector<W> &residual;

    std::vector<int> level;
    std::vector<int> q; // reuse as queue in dfs
    int s, t;

    dinic_ctx(graph<Sp, W> &g, int s, int t)
        : sp(g.sp), residual(std::get<0>(g.vx)), level(sp.r), q(sp.r), s(s), t(t) { }

    bool calc_level() {
        fill(level.begin(), level.end(), 0);
        int qh = 0, qt = 0;
        level[s] = 1;
        q[qt++] = s;

        while (qh < qt && !level[t]) {
            int u = q[qh++];

            for (auto [v, eid] : sp.out(u)) {
                if (!level[v] && residual[eid] > 0) {
                    level[v] = level[u] + 1;
                    q[qt++] = v;
                }
            }
        }
        return level[t];
    }

    WOut dfs(int u, WOut quota) {
        if (u == t) return quota;

        WOut pushed = 0;
        for (auto [v, eid] : sp.out(u)) {
            WOut r = residual[eid];
            if (level[v] == level[u] + 1 && r > 0) {
                WOut f = dfs(v, std::min(quota, r));
                residual[eid] -= f;
                residual[eid ^ 1] += f;
                quota -= f;
                pushed += f;
                // keep on going, do not stop on positive f
            }

            if (quota == 0) break;
        }
        return pushed;
    }

    WOut solve() {
        WOut pushed = 0;
        while (calc_level())
            pushed += dfs(s, std::numeric_limits<WOut>::max());
        return pushed;
    }
};

// specify WOut to overcome possible overflow
template <typename WOut, typename W, typename Sp>
WOut dinic(graph<Sp, W> &g, int s, int t) {
    return dinic_ctx<WOut, Sp, W>{g, s, t}.solve();
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_DINIC_H_ */
