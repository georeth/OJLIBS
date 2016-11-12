#ifndef OJLIBS_INC_DFS_TREE_H_
#define OJLIBS_INC_DFS_TREE_H_

#include <vector>
#include <tuple>
#include <ojlibs/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

void dfs_tree_time_helper(graph<> &g, std::vector<int> &discover, std::vector<int> &begin_time, std::vector<int> &end_time, int u, int prev, int &tnow) {
    discover[tnow] = u;
    begin_time[u] = tnow;
    ++tnow;
    for (auto &e : g.edge_list(u)) if (e.to != prev) {
        dfs_tree_time_helper(g, discover, begin_time, end_time, e.to, u, tnow);
    }
    end_time[u] = tnow;
}
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> dfs_tree_time(graph<> &g, int u, int &tnow) {
    std::vector<int> discover(g.vertex_size(), -1);
    std::vector<int> begin_time(g.vertex_size(), -1);
    std::vector<int> end_time(g.vertex_size(), -1);
    dfs_tree_time_helper(g, discover, begin_time, end_time, u, -1, tnow);
    return make_tuple(discover, begin_time, end_time);
}
    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_DFS_TREE_H_ */
