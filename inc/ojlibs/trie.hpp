#ifndef OJLIBS_INC_TRIE_H_
#define OJLIBS_INC_TRIE_H_

#include <string>
#include <deque>
#include <cstdint>

namespace ojlibs { // TO_BE_REMOVED

// char != unsigned char != signed char
// static_cast<int>(c) is implementation defined...

template <typename T, size_t Size, uint8_t Base>
struct trie_node {
    int id;
    T value;
    trie_node *c[Size];

    trie_node(int id, const T &v) : id(id), value(v) {
        std::fill(std::begin(c), std::end(c), nullptr);
    }

    trie_node *&next(uint8_t chr) {
        return c[chr - Base];
    }
};

template <typename T, size_t Size = 256, uint8_t Base = 0>
struct trie {
    typedef trie_node<T, Size, Base> node_t;

    std::deque<node_t> nodes;
    T init;

    trie(const T &init = T()) : init(init) {
        alloc(); // root
    }

    node_t *alloc() {
        nodes.push_back(node_t((int)nodes.size(), init));
        return &nodes.back();
    }

    T &operator[](const std::string &s) {
        node_t *cur = root();
        for (char c : s) {
            if (!cur->next(c))
                cur->next(c) = alloc();
            cur = cur->next(c);
        }
        return cur->value;
    }
    node_t *find(const std::string &s) {
        node_t *cur = root();
        for (char c : s) {
            if (!cur->next(c)) return nullptr;
            cur = cur->next(c);
        }
        return cur;
    }
    node_t *root() { return &nodes.front(); }
};
    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_TRIE_H_ */
