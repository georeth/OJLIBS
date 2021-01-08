#ifndef OJLIBS_INC_SPARSE_GRAPH_H_
#define OJLIBS_INC_SPARSE_GRAPH_H_

#include <cassert>
#include <vector>
#include <utility>
#include <ojlibs/iter_range.hpp>
#include <ojlibs/sparse/common.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename...E>
void param_pack_append(const std::tuple<std::vector<E> &...> &vx, E... es) {
    std::apply([es...](auto &... vx) {
        (vx.push_back(es),...);
    }, vx);
}

template <typename SP, typename... E>
struct graph {
    using iter_value_type = std::tuple<int, E...>;
    struct const_iterator : sp_iter_base<iter_value_type> {
        const graph &g;
        using sp_iter = typename SP::iter;
        sp_iter it;

        const_iterator(const graph &g, sp_iter it) : g(g), it(it) { }
        iter_value_type operator*() const {
            auto [j, eid] = *it;
            return std::apply([j, eid](auto &...t) {
                return iter_value_type{j, t[eid]...};
            }, g.vx);
        }
        const_iterator &operator++() { ++it; return *this; }
        bool operator==(const const_iterator &that) const { return it == that.it; }
        bool operator!=(const const_iterator &that) const { return !(*this == that); }
    };

    SP &sp;
    std::tuple<std::vector<E> &...> vx;

    graph(SP &sp, std::vector<E> &...vx)
        :sp(sp), vx(vx...) { (assert(vx.size() == sp.esize()),...); }

    int nvert() const { return sp.r; }
    int nvert2() const { return sp.c; } // for biparti graph / sparse matrix
    int nedge() const { return sp.esize(); }

    iter_range<const_iterator> out(int u) const {
        auto [b, e] = sp.out(u);
        return {{*this, b}, {*this, e}};
    }

    void add(int i, int j, E... es) {
        sp.add(i, j);
        param_pack_append(vx, es...);
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_GRAPH_H_ */
