#ifndef OJLIBS_INC_2SAT_H_
#define OJLIBS_INC_2SAT_H_

#include <cassert>
#include <vector>
#include <algorithm>
#include <ojlibs/scc_tarjan.hpp>

namespace ojlibs { // TO_BE_REMOVED
    // input a graph G=(V, E)
    // v_2k   denotes x
    // v_2k+1 denotes not x
    //
    // (x or y) becomes two edge (not x -> y) and (not y -> x)
    // do not forget it!
    template <typename EInfo>
    bool find_2sat(graph<EInfo> &g, std::vector<bool> &value_out) {
        assert(g.vertex_size() % 2 == 0);
        int nr_var = g.vertex_size() / 2;

        int nr_scc;
        std::vector<int> scc_id = scc_tarjan(g, nr_scc);

        std::vector<bool> assigned(g.vertex_size());
        std::vector<std::vector<int>> scc(nr_scc);

        for (int i = 0; i < nr_var; ++i) {
            if (scc_id[2 * i] == scc_id[2 * i + 1]) return false;
        }

        vector<bool> value(nr_var);
        for (int i = 0; i < 2 * nr_var; ++i)
            scc[scc_id[i]].push_back(i);

        for (int i = 0; i < nr_scc; ++i) {
            for (int u : scc[i]) {
                if (!assigned[u / 2]) {
                    assigned[u / 2] = true;
                    value[u / 2] = (u + 1) % 2;
                }
            }
        }
        value_out = value;
        return true;
    }
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_2SAT_H_ */
