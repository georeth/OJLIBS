#ifndef OJLIBS_INC_GRAPH_H_
#define OJLIBS_INC_GRAPH_H_

#include <vector>
#include <ojlibs/empty_type.hpp>
#include <ojlibs/int_range.hpp> // all_vertex
#include <ojlibs/index_range.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename EInfo = empty_type>
struct graph {
    struct edge_type {
        int next_index;
        int from;
        int to;
        EInfo info;
    };
    typedef index_iter<edge_type> iter_type;
    typedef typename iter_type::range_type range_type;

    int top = 0;
    std::vector<int> heads;
    std::vector<edge_type> edges;

    int vertex_size() { return heads.size(); }
    int edge_size() { return top; }
    void add_edge(int s, int t, const EInfo &info = EInfo()) {
        int edge_idx = top++;
        edge_type *e = &edges[edge_idx];
        e->from = s;
        e->to = t;
        e->info = info;
        e->next_index = heads[s];
        heads[s] = edge_idx;
    }
    graph(int max_v, int max_e) : heads(max_v, -1), edges(max_e) { }
    edge_type &edge(int edge_idx) { return edges[edge_idx]; }
    range_type edge_list(int start) { return iter_type::make_range(heads[start], edges.data()); }
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GRAPH_H_ */
