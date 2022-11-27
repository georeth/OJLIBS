#ifndef OJLIBS_INC_SPARSE_CUT_VERTEX_H_
#define OJLIBS_INC_SPARSE_CUT_VERTEX_H_

#include <vector>
#include <ojlibs/sparse/dfs.hpp>

namespace ojlibs { // TO_BE_REMOVED

struct cut_vertex_visitor : generic_dfs_visitor {
    int now = 0;
    std::vector<int> id, low;
    std::vector<int> cut_vertexes;
    int current_first = -1;
    int first_children = 0; // number of children in DFS rooting at current_first

    cut_vertex_visitor(int n): id(n), low(n) { }
    void pre_vert(int u, bool first) {
        id[u] = low[u] = now++;
        if (first) {
            current_first = u;
            first_children = 0;
        }
    }
    void post_vert(int u, bool first) {
        if (first && first_children > 1)
            cut_vertexes.push_back(u);
    }

    void pre_tree(int u, int v, int e) {
        if (u == current_first)
            ++first_children;
    }
    void post_tree(int u, int v, int e) {
        low[u] = min(low[u], low[v]);
        if (u != current_first && low[v] >= low[u]) {
            cut_vertexes.push_back(u);
        }
    }
    void post_back(int u, int v, int e) {
        // low[u] = min(low[u], low[v]);
        low[u] = min(low[u], id[v]);
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_SPARSE_CUT_VERTEX_H_
