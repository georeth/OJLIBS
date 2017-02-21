#ifndef OJLIBS_INC_LAZY_SEGTREE_H_
#define OJLIBS_INC_LAZY_SEGTREE_H_

#include <vector>
#include <ojlibs/bit_trick.hpp>
#include <ojlibs/binary_operator.hpp>
// TODO : early return (pruning)

// design : the order of lazy and agg is important.
// agg before lazy is better, just recompute agg from children.

namespace ojlibs { // TO_BE_REMOVED
// a top-down segment tree, combining of flavour I/II normal segment tree
// that support O(log N) range update and range query by using lazy propagation
//
// size is power of two, O(1) direct element query

// the functor F implements :
//      F() : identity function
//      T F(T agg, int len) : apply to a aggregated value of the whole range
//      F * F : function composition
template <typename T, typename Op, typename F>
struct lazy_segtree {
    // the real value of an element/range x is :
    //          get(x) = get(x / 2) op lazy[x / 2](vec[x])
    // note that lazy[x] is not included
    // in other words : parent - agg[x] - lazy[x] - child
    Op op;
    std::vector<T> vec;
    std::vector<F> lazy;

    int offset;
    int size; /* size of last layer */

    lazy_segtree(int n) {
        size = n;
        offset = ceil_pow2(n);
        vec.resize(offset + size + 1, op.identity()); /* ensure every leaf has sibling */
        lazy.resize(offset);
    }
    void update_range(int b, int e, const F &f) {
        if (b >= e) return; // always overlap
        return update_range_helper(1, 0, offset, b, e, f);
    }
    T query_range(int b, int e) {
        if (b >= e) return op.identity(); // always overlap
        return query_range_helper(1, 0, offset, b, e);
    }
    void rebuild() {
        for (int i = (offset + size - 1) / 2; i > 0; --i)
            vec[i] = op(vec[i + i], vec[i + i + 1]);
    }
    T &operator[](int p) {
        return vec[offset + p];
    }
private:
    void apply(int no, int len, const F &f) {
        vec[no] = f(vec[no], len);
        if (no < offset) lazy[no] = f * lazy[no];
    }
    // each `no' corresponding to a range [left, right[
    // b and e never changes in recursion
    void update_range_helper(int no, int left, int right, int b, int e, const F &f) {
        int len = right - left;
        // inside
        if (left >= b && right <= e)
            apply(no, len, f);
        // partial overlap
        else {
            if (lazy[no]) {
                apply(no + no, len / 2, lazy[no]);
                apply(no + no + 1, len / 2, lazy[no]);
                lazy[no] = F();
            }
            int mid = left + len / 2;
            if (b < mid) update_range_helper(no + no, left, mid, b, e, f);
            if (e > mid) update_range_helper(no + no + 1, mid, right, b, e, f);

            // child value may change, recalculate aggregated value
            vec[no] = op(vec[no + no], vec[no + no + 1]);
        }
    }
    T query_range_helper(int no, int left, int right, int b, int e) {
        int len = right - left;
        // inside
        if (left >= b && right <= e)
            return vec[no];
        // partial overlap
        else {
            if (lazy[no]) {
                apply(no + no, len / 2, lazy[no]);
                apply(no + no + 1, len / 2, lazy[no]);
                lazy[no] = F();
            }
            int mid = left + len / 2;
            T v1 = op.identity(), v2 = op.identity();
            if (b < mid) v1 = query_range_helper(no + no, left, mid, b, e);
            if (e > mid) v2 = query_range_helper(no + no + 1, mid, right, b, e);
            return op(v1, v2);
        }
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_LAZY_SEGTREE_H_ */
