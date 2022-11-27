#ifndef OJLIBS_INC_STRING_AC_AUTOMATON_H_
#define OJLIBS_INC_STRING_AC_AUTOMATON_H_

#include <ojlibs/string/trie.hpp>
#include <utility>

namespace ojlibs { // TO_BE_REMOVED

template <size_t Size, uint8_t Base>
struct ac_automaton {
    struct node_info_t {
        int fail = 0;
        int answer = -1;
        int cnt = 0; // #str share this suffix
    };
    typedef trie<node_info_t, Size, Base> trie_t;
    typedef typename trie_t::node_t node_t;
    trie_t tr;

    void add(const std::string &s, int value) {
        node_info_t &info = tr[s];
        info.answer = value;
        info.cnt += 1;
    }
    void build() {
        std::deque<node_t *> q;
        node_t *root = tr.root();

        // like kmp, start from the second charactor
        for (int c = 0; c < Size; ++c) {
            if (root->c[c]) q.push_back(root->c[c]);
        }

        while (q.size()) {
            node_t *u = q.front();
            q.pop_front();

            u->value.cnt += tr.nodes[u->value.fail].value.cnt;

            for (int c = 0; c < Size; ++c) {
                node_t *v = u->c[c];
                if (v) {
                    q.push_back(v);
                    node_t *p = &tr.nodes[u->value.fail];

                    while (p != root && !p->c[c])
                        p = &tr.nodes[p->value.fail];

                    if (p->c[c])
                        p = p->c[c];

                    v->value.fail = p->id;
                }
            }
        }
    }
    std::vector<int> traverse_id(const std::string &s) {
        std::vector<int> ret(s.size());
        node_t *cur = tr.root();
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            while (cur->id > 0 && !cur->next(c)) {
                cur = &tr.nodes[cur->value.fail];
            }

            if (cur->next(c)) cur = cur->next(c);
            ret[i] = cur->id;
        }
        return ret;
    }
    std::vector<int> traverse(const std::string &s) {
        std::vector<int> ret = traverse_id(s);
        for (int i = 0; i < ret.size(); ++i)
            ret[i] = tr.nodes[ret[i]].value.answer;
        return ret;
    }

};

    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_STRING_AC_AUTOMATON_H_ */
