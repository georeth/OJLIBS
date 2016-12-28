#ifndef OJLIBS_INC_PUSH_RELABEL
#define OJLIBS_INC_PUSH_RELABEL

#include <ojlibs/shorthand.hpp>
#include <ojlibs/graph.hpp>
#include <ojlibs/identity_visitor.hpp>

namespace ojlibs { // TO_BE_REMOVED

/* =====  SOLUTION START  ===== */
template <typename EInfo,
          typename WVisitor = identity_visitor<EInfo>>
struct push_relabel_solver {
    WVisitor visitor;
    typedef decltype(visitor.get(EInfo())) W;

    graph<EInfo> &g;
    int n;
    std::vector<int> level; // vertex -> level
    std::vector<W> excess; // vertex -> excessive value
    std::vector<bool> active; // active[v] = [excess[v] > 0]
    std::vector<std::vector<int>> bucket; // level -> { v : active[v] && level[v] = level }
    std::vector<int> count; // level -> #{ v : level[v] = level }
    int cur_level; // max non-empty bucket
    int s;

    push_relabel_solver(graph<EInfo> &g)
        : g(g), n(g.vertex_size()), level(n), 
          excess(n), active(n), bucket(n), count(n),
          cur_level(0) { }

    void relabel(int u) {
        --count[level[u]];
        level[u] = n;
        for (auto &e : g.edge_list(u)) {
            if (visitor.get(e.info))
                level[u] = min(level[u], level[e.to] + 1);
        }

        if (level[u] < n) {
            ++count[level[u]];
            enqueue(u);
        } else {
            excess[s] += excess[u];
            excess[u] = 0;
        }
    }

    void enqueue(int u) {
        if (!active[u] && excess[u] && level[u] < n) {
            active[u] = true;
            bucket[level[u]].push_back(u);

            cur_level = max(cur_level, level[u]);
        }
    }

    void gap(int lev) {
        for (int l = lev; l < n; ++l) {
            bucket[l].clear();
            count[l] = 0;
        }
        for (int i = 0; i < n; ++i) if (level[i] >= lev) {
            level[i] = n;
        }
    }

    void discharge(int u) {
        typename graph<EInfo>::range_type r = g.edge_list(u);
        for (auto it = r.first; it != r.second; ++it) {
            int v = it->to;
            if (excess[u] == 0) break;
            if (level[u] != level[v] + 1) continue;
            W f = min(visitor.get(it->info), excess[u]);
            if (!f) continue;

            visitor.get(it->info) -= f;
            visitor.get(g.edges[it.index() ^ 1].info) += f;
            excess[u] -= f;
            excess[v] += f;
            enqueue(v);
        }
        if (excess[u]) {
            if (count[level[u]] == 1)
                gap(level[u]);
            else
                relabel(u);
        }
    }

    W max_flow(int s, int t) {
        this->s = s;
        active[s] = true, active[t] = true; // never discharge
        count[0] = n; // large enought, never be gap

        // INIT
        level[s] = n; // never discharge to s first
        typename graph<EInfo>::range_type r = g.edge_list(s);
        for (auto it = r.first; it != r.second; ++it) {
            W f = visitor.get(it->info);
            visitor.get(it->info) = 0;
            visitor.get(g.edges[it.index() ^ 1].info) += f;
            excess[it->to] += f;

            enqueue(it->to);
        }

        // MAIN LOOP
        while (cur_level >= 0) {
            if (bucket[cur_level].size()) {
                int u = bucket[cur_level].back();
                bucket[cur_level].pop_back();
                active[u] = false;

                discharge(u);
            } else {
                --cur_level;
            }
        }
        return excess[t];
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_PUSH_RELABEL */
