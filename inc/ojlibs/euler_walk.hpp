#ifndef OJLIBS_INC_EULER_WALK_H_
#define OJLIBS_INC_EULER_WALK_H_

#include <utility>
#include <ojlibs/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

// do not forget:
//      1. check the existence
//              1.1 degree relation
//              1.2 connectivity
//      2. start from a good vertex

template <bool Directed>
struct eular_walk_solver {
    graph<bool> &g;

    typedef graph<bool> graph_type;
    eular_walk_solver(graph<bool> &g) : g(g) { }

    std::vector<int> answer;
    void solve_inner(int u) {
        graph_type::range_type ran = g.edge_list(u);
        for (graph_type::iter_type it = ran.begin(); it != ran.end(); ++it) {
            if (it->info) { // FIXME: need to remove the edge, otherwise time will be O(E^2)
                it->info = false;
                if (!Directed) g.edges[it.index() ^ 1].info = false;
                solve_inner(e.to);
            }
        }
    }
    std::vector<int> solve(int s) {
        solve_inner(s);
        return answer;
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_EULER_WALK_H_ */
