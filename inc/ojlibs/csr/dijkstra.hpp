#ifndef OJLIBS_INC_CSR_DIJKSTRA_H_
#define OJLIBS_INC_CSR_DIJKSTRA_H_

#include <ojlibs/csr/csr.hpp>
#include <ojlibs/csr/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename W>
struct dijkstra_info {
    std::vector<W> dist;
    std::vector<bool> visit;
    std::vector<int> pre;
};

template <typename W>
struct dijkstra_entry {
    int u, p;
    W d;

    bool operator< (const dijkstra_entry &that) const {
        // for priority queue
        return d > that.d;
    }
};

template <typename W, typename WOut=W>
dijkstra_info<W> dijkstra(const csr_graph<W> &g, int s) {
    int n = g.nvert();
    std::priority_queue<dijkstra_entry<WOut>> pq;

    std::vector<bool> visit(n);
    std::vector<WOut> dist(n, std::numeric_limits<WOut>::max());
    std::vector<int> pre(n, -1);

    pq.push({s, -1, 0});

    while (!pq.empty()) {
        auto [u, p, d] = pq.top();
        pq.pop();

        if (visit[u]) continue;
        dist[u] = d;
        visit[u] = true;
        pre[u] = p;

        for (auto [v, w] : g.out(u)) if (!visit[v]) {
            pq.push({v, u, d + w});
        }
    }
    return {dist, visit, pre};
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_CSR_DIJKSTRA_H_ */
