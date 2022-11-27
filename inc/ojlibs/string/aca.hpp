#ifndef OJLIBS_INC_STRING_ACA_H_
#define OJLIBS_INC_STRING_ACA_H_
#include <vector>
#include <deque>
#include <ojlibs/empty_type.hpp>
namespace ojlibs { // TO_BE_REMOVED

template <int Alpha = 256, typename Attr = empty_type>
struct trie_node : Attr {
     int p = 0;
     int cs[Alpha] = {}; // 0 is both root and null
     int fail = 0;
};

template <int Alpha, typename Attr = empty_type>
using trie = std::vector<trie_node<Alpha, Attr>>;

template <int Alpha, typename Attr>
int trie_walk(trie<Alpha, Attr> &tr, int prev, int c, bool insert = false) {
    // NEVER USE REFERENCE TO ELEMENT OF VECTOR!!!
    if (!insert || tr[prev].cs[c]) return tr[prev].cs[c];
    int cur = (int)tr.size();
    tr.push_back({});

    return tr[prev].cs[c] = cur;
}

template <int Alpha, typename Attr>
vector<vector<int>> ac_automaton_build(trie<Alpha, Attr> &tr) { // return aut matrix
    std::vector<std::vector<int>> aut(tr.size(), std::vector<int>(Alpha));
    const int root = 0;
    std::deque<int> q = {root}; // node in q has been computed

    while (!q.empty()) {
        int u = q.front(); q.pop_front();

        for (int c = 0; c < Alpha; ++c) {
            int v = tr[u].cs[c];
            if (v) {
                aut[u][c] = v;
                if (u != root)
                    tr[v].fail = aut[tr[u].fail][c];
                else
                    tr[v].fail = root;
                q.push_back(v);
            } else {
                if (u != root)
                    aut[u][c] = aut[tr[u].fail][c];
                else
                    aut[u][c] = root;
            }
        }
    }
    return aut;
}
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_STRING_ACA_H_ */
