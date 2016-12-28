#ifndef OJLIBS_INC_SEGTREE_H_
#define OJLIBS_INC_SEGTREE_H_

#include <vector>
#include <ojlibs/bit_trick.hpp>
#include <ojlibs/binary_operator.hpp>
namespace ojlibs { // TO_BE_REMOVED
// there are two flavours of normal segment tree:               TO_BE_REMOVED
//      update element, query range                             TO_BE_REMOVED
//   OR update range, query element                             TO_BE_REMOVED
// see lazy_segtree.hpp for a more powerful segment tree        TO_BE_REMOVED
// that supports all operations in time O(log N)                TO_BE_REMOVED
//                                                              TO_BE_REMOVED
// the operation must be commutative. if the result             TO_BE_REMOVED
// depends on the order, use lazy_segtree instead.              TO_BE_REMOVED
// (cannot implement for bottom-up segment tree)                TO_BE_REMOVED
#define COMMON_CODE \
    Op op; \
    std::vector<T> vec; \
    int size; /* size of last layer */ \
    int offset; \
    segtree(int n) { \
        size = n; \
        offset = ceil_pow2(n); \
        vec.resize(size + offset + 1, op.identity()); /* ensure every leaf has sibling */ \
    } \
    T &operator[](int i) { \
        return vec[offset + i]; \
    }

template <typename T, typename Op = binary_plus<T>, bool flavour_one = true>
struct segtree;

template <typename T, typename Op>
struct segtree<T, Op, true>
{
    // Flavour I : update element, query range (also query element by operator[])
    //          value(element) = vec[element+offset]
    //          vec[p] = op(vec[2p], vec[2p+1]) always holds
    COMMON_CODE
    void rebuild() {
        for (int i = (offset + size - 1) / 2; i > 0; --i)
            vec[i] = op(vec[i + i], vec[i + i + 1]);
    }
    void increase_element(int p, const T &n) {
        for (p += offset; p; p >>= 1)
            vec[p] = op(n, vec[p]);
    }
    void update_element(int p, const T &n) {
        p += offset;
        vec[p] = n;
        while (p >>= 1)
            vec[p] = op(vec[p + p], vec[p + p + 1]);
    }
    T query_range(int s, int t) {
        return query_include(s, t - 1);
    }
    T query_include(int s, int t) {
        // order is important if op is not commutative
        if (s > t) return op.identity();
        s += offset;
        t += offset;
        T left_acc = vec[s];
        if (s == t) return left_acc;
        T right_acc = vec[t];
        // query exclude ]s, t[ next
        for (; s^t^1; s>>=1, t>>=1) {
            if (~s & 1) left_acc = op(left_acc, vec[s^1]);
            if (t & 1) right_acc = op(vec[t^1], right_acc);
        }
        return op(left_acc, right_acc);
    }
};
template <typename T, typename Op>
struct segtree<T, Op, false>
{
    // Flavour II : query element, increase range (also increase element by operator[])
    //          Op must be commutative
    //          value(element) = sum{j}{vec[(element+offset)/2^j]} (aggregate all values above it)
    COMMON_CODE
    T query_element(int p){
        T ret = op.identity();
        for (p += offset; p; p >>= 1)
            ret = op(vec[p], ret);
        return ret;
    }
    void increase_range(int s, int t, const T &inc) {
        return increase_include(s, t - 1, inc);
    }
    void increase_include(int s, int t, const T &inc) {
        if (s > t) return;
        s += offset;
        t += offset;
        vec[s] = op(inc, vec[s]);
        if (s == t) return;
        vec[t] = op(inc, vec[t]);
        // query exclude ]s, t[ next
        for (; s^t^1; s>>=1, t>>=1) {
            if (~s & 1) vec[s^1] = op(inc, vec[s^1]);
            if (t & 1) vec[t^1] = op(inc, vec[t^1]);
        }
        return;
    }
    // apply internal nodes to leaves, so operator[] gets query_element
    void flatten() {
        for (int i = 1; i < offset; ++i) {
            if (i + i < offset + size) {
                vec[i + i] = op(vec[i], vec[i + i]);
                vec[i + i + 1] = op(vec[i], vec[i + i + 1]); // dummy sibling doesn't matter
            } else break;
            vec[i] = op.identity();
        }
    }
};
#undef COMMON_CODE
} // ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SEGTREE_H_ */
