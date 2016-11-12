#ifndef OJLIBS_INC_DIJKSTRA_H_
#define OJLIBS_INC_DIJKSTRA_H_

#include <queue>
#include <vector>
#include <utility>
#include <ojlibs/graph.hpp>
#include <ojlibs/identity_visitor.hpp>
namespace ojlibs { // TO_BE_REMOVED

// TODO prev_out
template <typename W, typename EInfo, typename WVisitor>
std::vector<W> dijkstra(graph<EInfo> &g, int start, std::vector<bool> *visit_out = nullptr) {
    std::vector<W> dist(g.vertex_size());
    std::vector<bool> visit(g.vertex_size());
    typedef std::pair<W, int> p_wu;
    std::priority_queue<p_wu, std::vector<p_wu>, std::greater<p_wu>> pq;
    pq.push(p_wu(0, start));

    while (!pq.empty()) {
        W w; int u;
        std::tie(w, u) = pq.top();
        pq.pop();

        if (visit[u]) continue;
        visit[u] = true;
        dist[u] = w;

        for (auto &e : g.edge_list(u)) {
            // do not skip visited (may be a shorter path)
            pq.push(p_wu(w + WVisitor::get(e.info), e.to));
        }
    }
    if (visit_out) *visit_out = visit;
    return dist;
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_DIJKSTRA_H_ */
