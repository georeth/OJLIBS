#ifndef OJLIBS_INC_GRAPH_H_
#define OJLIBS_INC_GRAPH_H_

#include <vector>
#include <deque>
#include <ojlibs/deprecated/empty_type.hpp>
#include <ojlibs/util/iter_range.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename EAttr = empty_type>
struct graph {
    typedef int Index;
    struct E : EAttr {
        Index from, to;
        E() { }
        E(Index from, Index to, const EAttr &attr)
            : EAttr(attr), from(from), to(to) { }
    };
    typedef typename std::deque<E> list_type;
    typedef typename list_type::iterator list_iter_type;
    struct target_iter : list_iter_type { // for (int v : g[u])
        // using list_iter_type::list_iter_type does not work TO_BE_REMOVED
        // because inherited copy constructor is hidden       TO_BE_REMOVED
        target_iter(const list_iter_type &base): list_iter_type(base) { }
        Index operator*() { return (*this)->to; }
    };

    graph(int n) : edges(n) { }
    void assign(int n) { edges.assign(n); }

    int size() { return (int)edges.size(); }
    void add(int u, int v, EAttr attr = EAttr()) {
        edges[u].push_back({u, v, attr});
    }
    iter_range<target_iter> operator[](int u) {
        return make_range(target_iter(edges[u].begin()), target_iter(edges[u].end()));
    }
    std::vector<list_type> edges; // to support erasion, no reallocation
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GRAPH_H_ */
