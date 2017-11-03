#ifndef OJLIBS_INC_ACA_H_
#define OJLIBS_INC_ACA_H_
#include <vector>
#include <deque>
namespace ojlibs { // TO_BE_REMOVED

template <int Alpha = 256, typename Attr = empty_type>
struct trie_node : Attr {
     int p = 0;
     int cs[Alpha] = {}; // 0 is both root and null
     int fail = 0;

     trie_node() {
         fill(ALL(cs), 0);
     }
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
void ac_automaton_build(trie<Alpha, Attr> &tr) {
    std::deque<int> q; // node in q has been computed
    for (int c = 0; c < Alpha; ++c)
        if (tr[0].cs[c]) q.push_back(tr[0].cs[c]);

    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        for (int c = 0; c < Alpha; ++c) {
            int v = tr[u].cs[c];
            if (v == 0) continue;

            q.push_back(v);
            int m = tr[u].fail;
            while (m > 0 && tr[m].cs[c] == 0) m = tr[m].fail;
            tr[v].fail = tr[m].cs[c];
        }
    }
}
template <int Alpha, typename Attr>
int ac_automaton_transit(trie<Alpha, Attr> &tr, int u, int c) {
    while (u > 0 && tr[u].cs[c] == 0) u = tr[u].fail;
    return tr[u].cs[c];
}
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ACA_H_ */
