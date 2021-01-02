#ifndef OJLIBS_INC_CSR_GRAPH_H_
#define OJLIBS_INC_CSR_GRAPH_H_

#include <ojlibs/csr/csr.hpp>
#include <ojlibs/iter_range.hpp>
#include <queue>

namespace ojlibs { // TO_BE_REMOVED

// concept graph<E...> {
//   int nvert();
//   Iter<tuple<int, E...>> out(int);
// }

template <typename... E>
struct csr_graph {
    struct const_iterator {
        typedef std::tuple<int, E...> value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef std::forward_iterator_tag iterator_category;

        const csr_graph &g;
        int idx;

        const_iterator(const csr_graph &g, int idx) : g(g), idx(idx) { }
        value_type operator*() const {
            int j = g.edges.jx[idx];
            return std::apply([&](auto &...t) {
                return value_type{j, t[idx]...};
            }, g.vx);
        }
        const_iterator &operator++() { ++idx; return *this; }
        bool operator==(const const_iterator &that) const { return  idx == that.idx; }
        bool operator!=(const const_iterator &that) const { return !(*this == that); }

    };
    csr &edges;
    std::tuple<std::vector<E> &...> vx;

    csr_graph(csr &edges, std::vector<E> &...vx)
        :edges(edges), vx(vx...) { }

    int nvert() const { return edges.r; }
    int nvert2() const { return edges.c; } // for biparti graph
    int nedge() const { return (int)edges.jx.size(); }
    iter_range<const_iterator> out(int u) const {
        return {const_iterator(*this, edges.ix[u]), const_iterator(*this, edges.ix[u + 1])};
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_CSR_GRAPH_H_ */
