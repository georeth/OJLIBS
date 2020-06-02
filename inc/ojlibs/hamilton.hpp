#ifndef OJLIBS_INC_HAMILTON_H_
#define OJLIBS_INC_HAMILTON_H_

// find hamiltonian cycle in an undirected graph satisfy Ore's theorem:
// i.e. any nonadjacent u and v, deg(u) + deg(v) >= n
//
// the key step to construct h-cycle incrementally:
// if we have a permutation of vertices arr[], for non-adjacent arr[i] and arr[i + 1],
// we can find (arr[k], arr[i+1]), (arr[k+1], arr[i]) in E.
// then we reverse arr[k:i+1], edges in arr[] increase at least one.
//
// this algorithm maintains a path, extends to maximal, then do the step.
// makes it easy to implement.
//
// Usage:
//      vector<int> hamilton = hamilton_solver(g, vertices).cycle;

#include <cassert>
#include <vector>
#include <algorithm>

struct hamilton_solver {
    std::vector<int> unvisited;
    const std::vector<std::vector<bool>> &g;
    std::vector<bool> vis;
    std::vector<int> cycle;

    void extend() {
        int t = cycle.back();
        while (true) {
            bool found = false;
            for (int idx = 0; idx < unvisited.size(); ++idx) {
                int v = unvisited[idx];
                if (g[t][v]) {
                    add_to_cycle(idx);
                    t = v;
                    found = true;
                    break;
                }
            }
            if (!found) return;
        }
    }

    hamilton_solver(const std::vector<std::vector<bool>> &g, const std::vector<int> &vertices) 
            : unvisited(vertices), g(g), vis(g.size()) {

        assert(vertices.size());
        solve();
    }
    void add_to_cycle(int idx) {
        int v = unvisited[idx];
        unvisited[idx] = unvisited.back();
        unvisited.pop_back();

        cycle.push_back(v);
        vis[v] = true;
    }
    void solve() {
        add_to_cycle(0);

        while (true) {
            // cycle is a path
            extend();
            std::reverse(cycle.begin(), cycle.end());
            extend();
            // cycle is a maximal path

            int s = cycle.front();
            int t = cycle.back();

            for (int i = 1; i + 1 < cycle.size() - 1; ++i) {
                if (g[s][cycle[i + 1]] && g[cycle[i]][t]) {
                    std::reverse(cycle.begin() + i + 1, cycle.end());
                    break;
                }
            }

            // cycle is a cycle
            if (unvisited.empty())
                return;

            for (int i = 0; i < cycle.size(); ++i) {
                bool found = false;
                for (int idx = 0; idx < unvisited.size(); ++idx) {
                    int j = unvisited[idx];
                    if (g[cycle[i]][j]) {
                        std::rotate(cycle.begin(), cycle.begin() + i + 1, cycle.end());
                        add_to_cycle(idx);
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }

            // cycle is a path
        }
    }
};

#endif /* end of include guard: OJLIBS_INC_HAMILTON_H_ */
