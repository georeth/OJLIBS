#ifndef OJLIBS_INC_SCC_TARJAN_H_
#define OJLIBS_INC_SCC_TARJAN_H_

#include <vector>
#include <ojlibs/sparse/scc.hpp>
#include <ojlibs/sparse/dfs.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename Sp>
scc_info scc_tarjan(const Sp &g) {
    struct vs : generic_dfs_visitor {
        std::vector<int> stack, scc_id, id, low;
        int now = 0, nr_scc = 0;

        vs(int n) : scc_id(n, -1), id(n), low(n) { }

        void pre_vert(int u, bool first) {
            id[u] = low[u] = now++;
            stack.push_back(u);
        }
        void post_all(int u, int v, int e) {
            // update only unclassified v (v in stack)
            if (scc_id[v] == -1)
                low[u] = std::min(low[u], low[v]);
        }
        void post_vert(int u, bool first) {
            if (id[u] == low[u]) {
                int v;
                do {
                    v = stack.back();
                    stack.pop_back();
                    scc_id[v] = nr_scc;
                } while (v != u);
                ++nr_scc;
            }
            // otherwise, leave u on stack.
            // this stack not only contains grey vertices,
            // but all visited but not classified vertices.
        }
    } visitor(g.r);

    generic_dfs(g, visitor, true);
    return {std::move(visitor.scc_id), std::move(visitor.nr_scc)};
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SCC_TARJAN_H_*/
