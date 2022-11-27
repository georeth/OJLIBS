#ifndef OJLIBS_INC_SPARSE_MAX_FLOW__H_
#define OJLIBS_INC_SPARSE_MAX_FLOW__H_

#include <vector>
#include <utility>
#include <limits>
#include <optional>
#include <ojlibs/sparse/graph.hpp>
#include <ojlibs/sparse/concrete_graph.hpp>
#include <ojlibs/sparse/dinic.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename WOut, typename W>
struct flow_info {
    bool valid;
    WOut size;
    std::vector<W> flow;
};

template <typename WNew>
struct residual_graph_info {
    concrete_graph<WNew> g;
    WNew full;
};

template <typename WNew, typename W, typename Sp>
residual_graph_info<WNew>
build_residual_graph(const Sp &sp, const vector<W> &low, const vector<W> &cap, bool has_low) {
    int n = has_low ? sp.r + 2 : sp.r;
    concrete_graph<WNew> new_g(n, n);

    std::vector<std::pair<int, int>> ijx(sp.esize());
    for (int u = 0; u < sp.r; ++u) {
        for (auto [v, eid] : sp.out(u))
            ijx[eid] = {u, v};
    }

    int ss = sp.r, tt = sp.r + 1; // used if has_low
    vector<WNew> balance(has_low ? sp.r : 0);

    for (int e = 0; e < sp.esize(); ++e) {
        auto [u, v] = ijx[e];
        new_g.add(u, v, cap[e] - low[e]);
        new_g.add(v, u, 0);

        if (has_low) {
            balance[u] -= low[e];
            balance[v] += low[e];
        }
    }

    WNew full = 0;
    if (has_low) {
        for (int u = 0; u < sp.r; ++u) {
            if (balance[u] > 0) {
                new_g.add(ss, u, balance[u]);
                new_g.add(u, ss, 0);
                full += balance[u];
            } else if (balance[u] < 0) {
                new_g.add(u, tt, -balance[u]);
                new_g.add(tt, u, 0);
            }
        }
    }
    return {std::move(new_g), full};
}

// edge = (low, cap)
template <typename WOut, typename W, typename Sp>
flow_info<WOut, W> bounded_max_flow(const graph<Sp, W, W> &g, int s, int t) {
    auto &sp = g.sp;
    auto &[low, cap] = g.vx;

    int ss = sp.r, tt = sp.r + 1;
    // because balance will sum up caps, use WOut to avoid overflow
    auto [new_g, full] = build_residual_graph<WOut>(sp, low, cap, true);

    // feasible flow can break balance of s, t
    int ts_edge = new_g.add(t, s, std::numeric_limits<WOut>::max());
    new_g.add(s, t, 0);

    auto [old_wx] = new_g.vx;
    WOut size = dinic<WOut>(new_g.get(), ss, tt); // first dinic gets feasible flow
    if (size != full)
        return {false, 0, {}};
    auto &[new_wx] = new_g.vx;

    size = dinic<WOut>(new_g, s, t); // second dinic gets max flow

    std::vector<W> flow(sp.esize());
    for (int e = 0; e < sp.esize(); ++e) {
        flow[e] = low[e] + old_wx[e * 2] - new_wx[e * 2];
    }

    return {true, size + old_wx[ts_edge] - new_wx[ts_edge], std::move(flow)};
}


// edge = (low, cap)
template <typename WOut, typename W, typename Sp>
flow_info<WOut, W> bounded_feasible_flow(const graph<Sp, W, W> &g) {
    auto &sp = g.sp;
    auto &[low, cap] = g.vx;

    int ss = sp.r, tt = sp.r + 1;
    // because balance will sum up caps, use WOut to avoid overflow
    auto [new_g, full] = build_residual_graph<WOut>(sp, low, cap, true);

    auto [old_wx] = new_g.vx;
    WOut size = dinic<WOut>(new_g.get(), ss, tt);
    if (size != full)
        return {false, 0, {}};
    auto &[new_wx] = new_g.vx;

    std::vector<W> flow(sp.esize());
    for (int e = 0; e < sp.esize(); ++e) {
        flow[e] = low[e] + old_wx[e * 2] - new_wx[e * 2];
    }

    return {true, 0, std::move(flow)};
}

template <typename WOut, typename W, typename Sp>
flow_info<WOut, W> max_flow(const graph<Sp, W> &g, int s, int t) {
    auto &sp = g.sp;
    auto &[cap] = g.vx;

    auto [new_g, full] = build_residual_graph<WOut>(sp, {}, cap, true);

    auto [old_wx] = new_g.vx;
    WOut size = dinic<WOut>(new_g.get(), s, t);
    auto &[new_wx] = new_g.vx;

    std::vector<W> flow(sp.esize());
    for (int e = 0; e < sp.esize(); ++e) {
        flow[e] = old_wx[e * 2] - new_wx[e * 2];
    }

    return {true, size, std::move(flow)};
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_DINIC_H_ */
