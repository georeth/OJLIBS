#ifndef OJLIBS_INC_MICS_TWO_SAT_H_
#define OJLIBS_INC_MICS_TWO_SAT_H_
#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/scc_tarjan.hpp>

namespace ojlibs { // TO_BE_REMOVED

struct two_sat_info {
    bool valid;
    std::vector<bool> values;
};

struct two_sat {
    int nvar;
    lil g;
    two_sat(int nvar) : nvar(nvar), g(2 * nvar) { }

    // add constraint (x1 \/ x2)
    // input x -> variable u -> graph vertex index v
    // u  -> u     -> 2u
    // ~u -> not u -> 2u + 1
    void add(int x1, int x2) {
        int v1 = x1 < 0 ? 1 + 2 * ~x1 : 2 * x1;
        int v2 = x2 < 0 ? 1 + 2 * ~x2 : 2 * x2;

        g.add(v1 ^ 1, v2);
        g.add(v2 ^ 1, v1);
    }

    two_sat_info solve() {
        auto [sid, nr_scc] = scc_tarjan(g);

        for (int u = 0; u < nvar; ++u)
            if (sid[2 * u] == sid[2 * u + 1])
                return {false, {}};

        std::vector<std::vector<int>> scc(nr_scc);
        for (int v = 0; v < 2 * nvar; ++v)
            scc[sid[v]].push_back(v);

        std::vector<bool> value(nvar), assigned(nvar);
        for (int s = 0; s < nr_scc; ++s) {
            for (int v : scc[s]) {
                int u = v / 2;
                if (!assigned[u]) {
                    // by topological sort, v has no out-edge, not v no in-edge
                    // set v to true, not v to false
                    // (any -> v) and (not v -> any) are satisfied
                    // remove v and not v, reduce to smaller problem
                    assigned[u] = true;
                    value[u] = ~v & 1;
                }
            }
        }
        return {true, std::move(value)};
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_MICS_TWO_SAT_H_
