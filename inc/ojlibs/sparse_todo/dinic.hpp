#ifndef OJLIBS_INC_DINIC_H_
#define OJLIBS_INC_DINIC_H_

#include <ojlibs/identity_visitor.hpp>
#include <ojlibs/graph.hpp>
#include <queue>
#include <limits>

namespace ojlibs { // TO_BE_REMOVED

// time: O(V^2 E)
// unit capacity: O(min(V^(2/3), E^(1/2)) E)
// biparti-mathcing: O(sqrt(V) E)
template <typename Flow>
struct dinic_max_flow {
    typedef int Index;
    struct attr {
        Flow f;
        Index rev;
    };
    graph<attr> g;
    dinic_max_flow(int max_vertex) : g(max_vertex) { }
    int add(Index from, Index to, Flow cap, Flow rev_cap = 0) { // input a residue graph
        if (from == to) return -1;
        attr e1 = {cap, (int)g.edges[to].size()};
        attr e2 = {rev_cap, (int)g.edges[from].size()};
        int ret = (int)g.edges[from].size();
        g.add(from, to, e1);
        g.add(to, from, e2);
        return ret;
    }
    bool dinic_bfs(Index s, Index t, std::vector<int> &level) {
        std::vector<int> q(g.size()); // fixed size vector is faster
        int qh = 0, qt = 0;
        level[s] = 1;
        q[qt++] = s;

        while (qh < qt && !level[t]) {
            int u = q[qh++];

            for (auto &e : g.edges[u]) {
                int v = e.to;
                if (!level[v] && e.f > 0) {
                    level[v] = level[u] + 1;
                    q[qt++] = v;
                }
            }
        }
        return level[t];
    }
    Flow dinic_dfs(Index u, Index t, Flow flow, const std::vector<int> &level) {
        if (u == t) return flow;
        Flow pushed = 0;
        for (auto &e : g.edges[u]) {
            if (!flow) break;
            int v = e.to;
            Flow w = e.f;
            if (level[v] == level[u] + 1 && w) {
                Flow f = dinic_dfs(v, t, std::min(flow, w), level);
                e.f -= f;
                g.edges[v][e.rev].f += f;
                flow -= f;
                pushed += f;
                // keep on going, do not stop on positive f
            }
        }
        return pushed;
    }
    Flow max_flow(int s, int t) {
        Flow flow = 0;
        std::vector<int> level(g.size()); // level[s] = 1
        while (dinic_bfs(s, t, level)) {
            // update blocked flow in a single dfs!!
            // 0.37s vs TLE(> 2.76s) in SPOJ FASTFLOW
            // I've seen so many blogs on DINIC doing dfs in a loop
            flow += dinic_dfs(s, t, std::numeric_limits<Flow>::max(), level);
            fill(level.begin(), level.end(), 0);
        }
        return flow;
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_DINIC_H_ */
