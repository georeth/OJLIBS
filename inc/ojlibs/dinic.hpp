#ifndef OJLIBS_INC_DINIC_H_
#define OJLIBS_INC_DINIC_H_

#include <ojlibs/identity_visitor.hpp>
#include <ojlibs/graph.hpp>
#include <queue>
#include <limits>

namespace ojlibs { // TO_BE_REMOVED

template<typename EInfo, typename WVisitor>
bool dinic_bfs(graph<EInfo> &g, int s, int t, std::vector<int> &level) {
    std::vector<int> q(g.vertex_size()); // fixed size vector is faster
    int qh = 0, qt = 0;
    level[s] = 1;
    q[qt++] = s;

    while (qh < qt && !level[t]) {
        int u = q[qh++];

        for (auto &e : g.edge_list(u)) {
            int v = e.to;
            if (!level[v] && WVisitor::get(e.info) > 0) {
                level[v] = level[u] + 1;
                q[qt++] = v;
            }
        }
    }
    return level[t];
}

template<typename W, typename EInfo, typename WVisitor>
W dinic_dfs(graph<EInfo> &g, int u, int t, int64_t flow, std::vector<int> &level) {
    if (u == t) return flow;
    W pushed = 0;
    typename graph<EInfo>::range_type range = g.edge_list(u);
    for (auto it = range.first; it != range.second; ++it) {
        if (!flow) break;
        int v = it->to;
        W w = WVisitor::get(it->info);
        if (level[v] == level[u] + 1 && w) {
            W f = dinic_dfs<W, EInfo, WVisitor>(g, v, t, std::min(flow, w), level);
            WVisitor::get(it->info) -= f;
            WVisitor::get(g.edges[it.index() ^ 1].info) += f;
            flow -= f;
            pushed += f;
            // keep on going, do not stop on positive f
        }
    }
    return pushed;
}

// requirement of graph : edge[2k] = {u, v} then  edge[2k+1] = {v, u}
template<typename W, typename EInfo, typename WVisitor = identity_visitor<EInfo>>
W dinic(graph<EInfo> &g, int s, int t) {
    W flow = 0;
    std::vector<int> level(g.vertex_size()); // level[s] = 1
    while (dinic_bfs<EInfo, WVisitor>(g, s, t, level)) {
        // update blocked flow in a single dfs!!
        // 0.37s vs TLE(> 2.76s) in SPOJ FASTFLOW
        // I've seen so many blogs on DINIC doing dfs in a loop
        flow += dinic_dfs<W, EInfo, WVisitor>(g, s, t, std::numeric_limits<W>::max(), level);
        fill(level.begin(), level.end(), 0);
    }
    return flow;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_DINIC_H_ */
