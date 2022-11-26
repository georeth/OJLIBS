#ifndef OJLIBS_INC_SPARSE_SCC_KOSARAJU_H_
#define OJLIBS_INC_SPARSE_SCC_KOSARAJU_H_

#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/dfs.hpp>
#include <ojlibs/sparse/scc.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename Sp>
scc_info scc_kosaraju(const Sp &g) {
    // construct inverse graph G^T
    assert(g.r == g.c);
    lil gt(g.c, g.r);
    for (int u = 0; u < g.r; ++u)
        for (int v : g.outv(u))
            gt.add(v, u);

    // get post from G^T
    struct post_t : generic_dfs_visitor {
        std::vector<int> post;
        void post_vert(int u) { post.push_back(u); }
    } post_v;
    generic_dfs(gt, post_v, true);

    // dfs in reversed post in G
    struct fill_t : generic_dfs_visitor {
        int value = 0;
        std::vector<int> vec;
        fill_t(int n) : vec(n) { }
        void pre_vert(int u) { vec[u] = value; }
    } fill_v(g.r);

    std::vector<int> visit(g.r);
    generic_dfs_ctx ctx{g, fill_v, visit, true};
    for (int idx = g.r - 1; idx >= 0; --idx) {
        int u = post_v.post[idx];
        if (!visit[u]) {
            generic_dfs_visit(ctx, u, -1);
            ++fill_v.value;
        }
    }

    return {std::move(fill_v.vec), std::move(fill_v.value)};
}


} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SPARSE_SCC_KOSARAJU_H_ */
