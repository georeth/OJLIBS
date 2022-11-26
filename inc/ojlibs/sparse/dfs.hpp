#ifndef OJLIBS_INC_SPARSE_DFS_H_
#define OJLIBS_INC_SPARSE_DFS_H_

#include <vector>
#include <cassert>

namespace ojlibs { // TO_BE_REMOVED

struct generic_dfs_visitor {
    void pre_vert(int u, bool first) { }
    void post_vert(int u, bool first) { }

    void pre_tree(int u, int v, int e) { }
    void post_tree(int u, int v, int e) { }
    void post_back(int u, int v, int e) { }
    void post_fwd_cross(int u, int v, int e) { }
    void post_all(int u, int v, int e) { }
};

template <typename Sp, typename Vs>
struct generic_dfs_ctx {
    const Sp &sp;
    Vs &visitor;
    std::vector<int> &visit;
    bool directed;

    // for CTAD
    generic_dfs_ctx(const Sp &sp, Vs &visitor, std::vector<int> &visit, bool directed)
        : sp(sp), visitor(visitor), visit(visit), directed(directed) { }
};

template <typename Sp, typename Vs>
void generic_dfs_visit(const generic_dfs_ctx<Sp, Vs> &ctx, int u, int p) {
    auto &[sp, vs, visit, directed] = ctx;
    assert(!visit[u]);

    visit[u] = 1;
    vs.pre_vert(u, p == -1);
    for (auto [v, e] : sp.out(u)) if (directed || v != p) {
        if (!visit[v]) {
            vs.pre_tree(u, v, e);
            generic_dfs_visit(ctx, v, u);
            vs.post_tree(u, v, e);
        } else if (visit[v] == 1) {
            vs.post_back(u, v, e);
        } else {
            vs.post_fwd_cross(u, v, e);
        }
        vs.post_all(u, v, e);
    }
    vs.post_vert(u, p == -1);
    visit[u] = 2;
}

template <typename Sp, typename Vs>
void generic_dfs(const Sp &sp, Vs &visitor, bool directed) {
    int n = sp.r;
    std::vector<int> visit(n);

    generic_dfs_ctx<Sp, Vs> ctx{sp, visitor, visit, directed};
    for (int u = 0; u < n; ++u) if (!visit[u]) {
        generic_dfs_visit(ctx, u, -1);
    }
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_SPARSE_DFS_H_
