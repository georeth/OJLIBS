#ifndef OJLIBS_INC_SPARSE_DFS_ITERATIVE_H_
#define OJLIBS_INC_SPARSE_DFS_ITERATIVE_H_

#include <vector>

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
    int u, stg, p;
};

template <typename Sp>
dfs_info depth_first_search(const Sp &g, bool directed) {
    assert(g.r == g.c);
    dfs_info info(g.r);
    std::vector<dfs_entry> stk;
    std::vector<int> visit(g.r);

    for (int u = 0; u < g.r; ++u) if (!visit[u]) {
        stk.push_back({u, 0, -1});

        while (!stk.empty()) {
            auto &[u, stg, p] = stk.back();
            if (stg == 1) {
                assert(visit[u] == 1);
                visit[u] = 2;
                info.post.push_back(u);
                stk.pop_back();
                continue;
            }

            if (visit[u]) {
                assert(visit[u] == 2);
                info.fwd_cross = true;
                stk.pop_back();
                continue;
            }


            /*
            if (visit[u]) {
                if (stg == 1) {
                    assert(visit[u] == 1);
                    visit[u] = 2;
                    info.post.push_back(u);
                } else { // stg == 0
                    info.fwd_cross = true;
                }

                stk.pop_back();
                continue;
            }
            */

            visit[u] = 1;
            info.pre.push_back(u);
            info.parent[u] = p;
            ++stg;

            for (auto v : g.outv(u)) if (directed || v != p) {
                if (!visit[v])
                    stk.push_back({v, 0, u});
                else if (visit[v] == 1)
                    info.back = true;
                else {
                    // we can also push this vert (more canonical)
                    // but it waste time
                    assert(visit[v] == 2);
                    info.fwd_cross = true; // we can skip push, but we have to set fwd_cross
                }

            }
        }
    }
    assert(info.post.size() == g.r);
    assert(info.pre.size() == g.r);
    return info;
}

template <typename Sp>
struct dfs_entry_it {
    int u;
    // typename Sp::vert_iter it, it_e;
    int it, it_e;
};

template <typename Sp>
dfs_info depth_first_search_it(const Sp &g, bool directed) {
    assert(g.r == g.c);
    dfs_info info(g.r);
    std::vector<dfs_entry_it<Sp>> stk;
    stk.reserve(g.r);
    std::vector<int> visit(g.r);

    for (int u = 0; u < g.r; ++u) if (!visit[u]) {
        visit[u] = 1;
        info.pre.push_back(u);
        info.parent[u] = -1;
        stk.push_back({u, g.ix[u], g.ix[u + 1]});

        while (!stk.empty()) {
            auto &[u, it, it_e] = stk.back();
            if (it == it_e) {
                assert(visit[u] == 1);
                visit[u] = 2;
                info.post.push_back(u);
                stk.pop_back();
                continue;
            }

            int v = g.jx[it];
            ++it;
            if (!directed && v == info.parent[u])
                continue;

            if (!visit[v]) {
                visit[v] = 1;
                info.pre.push_back(v);
                info.parent[v] = u;
                // auto &&[vit, vit_e] = g.outv(v);
                stk.push_back({v, g.ix[v], g.ix[v + 1]});
            } else if (visit[v] == 1) {
                info.back = true;
            } else if (visit[v] == 2) {
                info.fwd_cross = true;
            }
        }
    }
    assert(info.post.size() == g.r);
    assert(info.pre.size() == g.r);
    return info;
}


} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_SPARSE_DFS_ITERATIVE_H_
