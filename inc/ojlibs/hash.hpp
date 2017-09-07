#ifndef OJLIBS_INC_HASH_H_
#define OJLIBS_INC_HASH_H_

#include <cstddef>
#include <functional>
namespace ojlibs { // TO_BE_REMOVED

void hash_combine(size_t &seed, size_t v) { // from BOOST
    seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
struct ojlibs_hash {
    // forward to std::hash
    template <typename T>
    size_t operator()(const T &v) {
        return std::hash<T>()(v);
    }
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2> &v) {
        size_t r = (*this)(v.first);
        hash_combine(r, (*this)(v.second));
        return r;
    }
};
// positional hash: each position has a weight. combine by plus or xor.
//      thus we can update hash value.
// polynomial hash: position i has weight p^i. so we can shift it.
//      and compute hash for each substring.
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_HASH_H_ */
