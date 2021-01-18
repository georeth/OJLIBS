#ifndef OJLIBS_INC_SPARSE_CONCRETE_GRAPH_H_
#define OJLIBS_INC_SPARSE_CONCRETE_GRAPH_H_

#include <cassert>
#include <vector>
#include <tuple>
#include <ojlibs/sparse/lil.hpp>
#include <ojlibs/sparse/common.hpp>
#include <ojlibs/sparse/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename... E>
struct concrete_graph {
    lil sp;
    std::tuple<std::vector<E>...> vx;

    concrete_graph(const concrete_graph &that) = delete;
    concrete_graph(concrete_graph &&that) = default;
    concrete_graph(int r, int c) : sp(r, c) { }

    graph<lil, E...> get() {
        return std::apply([this](auto &... ele) {
            return graph<lil, E...>(sp, ele...);
        }, vx);
    }

    int add(int i, int j, E... es) {
        int eid = sp.add(i, j);
        param_pack_append(vx, es...);
        return eid;
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_CONCRETE_GRAPH_H_ */
