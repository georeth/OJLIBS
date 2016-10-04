#ifndef OJLIBS_INC_GRAPH_H_
#define OJLIBS_INC_GRAPH_H_

#include <vector>
#include <ojlibs/empty_type.hpp>
#include <ojlibs/slist.hpp>
#include <ojlibs/int_range.hpp> // all_vertex

namespace ojlibs { // TO_BE_REMOVED
namespace data_structure { // TO_BE_REMOVED

template <typename EInfo = empty_type,
          typename Head = slist_head>
struct graph {
    typedef intrusive_traits<Head> traits_type;
    typedef typename traits_type::head_type head_type;
    struct edge_t {
        head_type list;
        int to;
        EInfo info;
    };
    typedef typename traits_type::template iter_type<edge_t, &edge_t::list> iter_type;
    typedef typename iter_type::range_type range_type;

    int top = 0;
    std::vector<head_type> heads;
    std::vector<edge_t> edges;

    range_t all_vertex() { return int_range(heads.size()); }
    void add_edge(int s, int t, const EInfo &info = EInfo()) {
        edge_t *e = &edges[top++];
        e->to = t;
        e->info = info;
        heads[s].insert_next(e->list);
    }
    graph(int max_v, int max_e) : heads(max_v), edges(max_e) { }
    range_type edge_list(int start) { return iter_type::make_range(heads[start]); }
};

} // data_structure TO_BE_REMOVED
} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GRAPH_H_ */
