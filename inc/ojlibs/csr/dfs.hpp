#ifndef OJLIBS_INC_CSR_DFS_H_
#define OJLIBS_INC_CSR_DFS_H_

#include <ojlibs/csr/graph.hpp>
#include <ojlibs/vector.hpp>

namespace ojlibs { // TO_BE_REMOVED

struct dfs_info {
    std::vector<int> pre, post, parent;
    bool back = false;
    bool fwd_cross = false;

    dfs_info() = default;
    dfs_info(int n) : parent(n, -1) {
        pre.reserve(n);
        post.reserve(n);
    }
};

struct dfs_entry {
    int v, stg, p;
};

dfs_info depth_first_search(const csr_graph<> &g, bool directed) {
    dfs_info info(g.nvert());
    std::vector<dfs_entry> stk;
    std::vector<int> visit(g.nvert());
    int now = 0;

    for (int u = 0; u < g.nvert(); ++u) if (!visit[u]) {
        stk.push_back({u, 0, -1});

        while (!stk.empty()) {
            auto [u, stg, p] = stk.back();
            if (visit[u]) {
                if (stg == 1) {
                    assert(visit[u] == 1);
                    visit[u] = 2;
                    info.post.push_back(u);
                }

                stk.pop_back();
                continue;
            } else {
                info.fwd_cross = true;
            }

            visit[u] = 1;
            info.pre.push_back(u);
            info.parent[u] = p;
            ++stk.back().stg;

            for (auto [v] : g.out(u)) if (directed || v != p) {
                if (!visit[v])
                    stk.push_back({v, 0, u});
                else if (visit[v] == 1)
                    info.back = true;
            }
        }
    }
    assert(info.post.size() == g.nvert());
    assert(info.pre.size() == g.nvert());
    return info;
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_CSR_DFS_H_
