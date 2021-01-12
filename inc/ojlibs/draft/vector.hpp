#ifndef OJLIBS_INC_vector_H_
#define OJLIBS_INC_vector_H_
#include <cassert>
#include <vector>

namespace ojlibs { // TO_BE_REMOVED

// there are alternative definitions for permutation related concepts,
// we choose the one matching our intuition, so we make less mistakes.
//
// permutation <-> vector 
//   we use the `one-line notation`
//        p(i)  =  p[i]
//
// composite(p1, p2)
//      (p1 o p2) = composite(p1, p2)
//      (p1 o p2) (i) = p1(p2(i))
//
// permutation p <-> permutation matrix P = perm_mat(p)
//        p(i) <=> P(p(i), i) = 1
//   so that:
//        inverse(p) <=> transpose(P) = inverse(P)
//        p o p2 = P * p2 (LHS is compotite, RHS is matrix-vector multiply)
//
// apply p to vector u:
//      v = apply(p, u) = perm_mat(p) * u
//
//   this is quite confusing: one may write p(u)
//   there is something wrong, since u:index->value p:index->index.
//   we choose the simple definition
//      v = apply(p, u) = u[p] = composite(u, p)
//   (using a index-vector p as index to u)

std::vector<int> inverse(const std::vector<int> &v) {
    std::vector<int> ret(v.size());
    for (int i = 0; i < v.size(); ++i) {
        assert(v[i] >= 0 && v[i] < v.size());
        ret[v[i]] = i;
    }
    return ret;
}

// v[idx]
template <typename T>
std::vector<T> composite(const std::vector<T> &v, const std::vector<int> &idx) {
    std::vector<T> ret(idx.size());
    for (size_t i = 0; i < idx.size(); ++i) {
        ret[i] = v[idx[i]];
    }
    return ret;
}
// v[idx^{-1}]
template <typename T>
std::vector<T> composite_rev(const std::vector<T> &v, const std::vector<int> &idx) {
    std::vector<T> ret(idx.size());
    for (size_t i = 0; i < idx.size(); ++i) {
        ret[idx[i]] = v[i];
    }
    return ret;
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_vector_H_
