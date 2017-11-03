#ifndef OJLIBS_INC_SCC_TARJAN_H_
#define OJLIBS_INC_SCC_TARJAN_H_

#include <vector>
#include <ojlibs/graph.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename EInfo>
struct scc_tarjan_solver {
    graph<EInfo> &g;
    std::vector<bool> onstack;
    std::vector<int> stack;
    std::vector<int> answer;

    std::vector<int> id;
    std::vector<int> low;
    int now;
    int nr_scc;

    scc_tarjan_solver(graph<EInfo> &g) : g(g) {
        onstack.resize(g.size());
        answer.resize(g.size(), -1);

        id.resize(g.size(), -1);
        low.resize(g.size());

        now = 0;
        nr_scc = 0;
    }

    std::vector<int> solve(int &nr_scc_out) {
        for (int u = 0; u < g.size(); ++u)
            if (answer[u] == -1)
                dfs(u);
        nr_scc_out = nr_scc;
        return answer;
    }

    void dfs(int u) {
        id[u] = now;
        low[u] = now;
        ++now;

        stack.push_back(u);
        onstack[u] = true;

        for (int v : g[u]) {
            if (id[v] == -1) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            } else if (onstack[v]) {
                // the original
                //      low[u] = std::min(low[u], id[v]);
                // is also correct, but i think this is more clear
                low[u] = std::min(low[u], low[v]);
            }
        }
        if (low[u] == id[u]) {
            int v;
            do {
                v = stack.back();
                stack.pop_back();
                answer[v] = nr_scc;

                onstack[v] = false;
            } while (v != u);
            ++nr_scc;
        }

    }
};

template <typename EInfo>
std::vector<int> scc_tarjan(graph<EInfo> &g, int &nr_scc) {
    return scc_tarjan_solver<EInfo>(g).solve(nr_scc);
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SCC_TARJAN_H_*/
