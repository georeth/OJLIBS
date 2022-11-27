#ifndef OJLIBS_INC_SCC_H_
#define OJLIBS_INC_SCC_H_

#include <ojlibs/graph.hpp>
#include <vector>

// TODO : move dfs_finish_order, propagate_unvisit, propagate_all to new header file
namespace ojlibs { // TO_BE_REMOVED

template <typename EInfo>
void dfs_finish_order_helper(graph<EInfo> &g, int u, std::vector<int> &visit, std::vector<int> &result) {
    visit[u] = 1;
    for (auto &e : g.edge_list(u)) {
        if (!visit[e.to])
            dfs_finish_order_helper(g, e.to, visit, result);
    }
    visit[u] = 2;
    result.push_back(u);
}
template <typename EInfo>
std::vector<int> dfs_finish_order(graph<EInfo> &g) {
    std::vector<int> result;
    std::vector<int> visit(g.vertex_size());
    for (int u = 0; u < g.vertex_size(); ++u) if (!visit[u])
        dfs_finish_order_helper(g, u, visit, result);
    return result;
}
template <typename EInfo>
void propagate_unvisit(graph<EInfo> &g, int u, int rep, int unvisit, std::vector<int> &visit) {
    visit[u] = rep;
    for (auto &e : g.edge_list(u))
        if (visit[e.to] == unvisit)
            propagate_unvisit(g, e.to, rep, unvisit, visit);
}
template <typename EInfo, typename EInfoT>
std::vector<int> scc_kosaraju(graph<EInfo> &g, graph<EInfoT> gt, int &nr_scc) {
    // gt is transpose graph of g (no template argument)
    int top = 0;
    std::vector<int> fo = dfs_finish_order(g);
    std::vector<int> result(g.vertex_size(), -1);
    for (int ri = fo.size() - 1; ri >= 0; --ri) {
        int u = fo[ri];
        if (result[u] == -1) propagate_unvisit(gt, u, ++top, -1, result);
    }
    nr_scc = top;
    return result;
}
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SCC_H_ */
