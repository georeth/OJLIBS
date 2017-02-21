#ifndef OJLIBS_INC_IMPLICIT_SEGTREE_H_
#define OJLIBS_INC_IMPLICIT_SEGTREE_H_

#include <deque>
#include <ojlibs/bit_trick.hpp>
#include <ojlibs/binary_operator.hpp>

namespace ojlibs { // TO_BE_REMOVED

#define COMMON_CODE \
    Op op; \
    struct node_t { \
        int b, e; \
        T val; \
        node_t *left = nullptr, *right = nullptr; \
        node_t() { } \
        node_t(int b, int e, const T &val) : b(b), e(e), val(val) { } \
    }; \
    std::deque<node_t> pool; \
private: \
    node_t *get_left(node_t *cur, int m) { \
        if (!cur->left) { \
            pool.push_back(node_t(cur->b, m, op.identity())); \
            cur->left = &pool.back(); \
        } \
        return cur->left; \
    } \
    node_t *get_right(node_t *cur, int m) { \
        if (!cur->right) { \
            pool.push_back(node_t(m, cur->e, op.identity())); \
            cur->right = &pool.back(); \
        } \
        return cur->right; \
    } \
public: \
    implicit_segtree(int b, int e) { \
        if (e - b < 1) throw 0; \
        pool.push_back(node_t(b, e, op.identity())); \
    }

template <typename T, typename Op = binary_plus<T>, bool flavour_one = true>
struct implicit_segtree;

template <typename T, typename Op>
struct implicit_segtree<T, Op, true>
{
    // Flavour I : update element, query range (also query element by operator[])
    //          value(element) = vec[element+offset]
    //          vec[p] = op(vec[2p], vec[2p+1]) always holds
    COMMON_CODE

    void increase_element(int p, const T &n) {
        node_t *root = &pool.front();
        if (p < root->b || p >= root->e) throw 0;
        return increase_element_inner(root, p, n);
    }
    void update_element(int p, const T &n) {
        node_t *root = &pool.front();
        if (p < root->b || p >= root->e) throw 0;
        return update_element_inner(root, p, n);
    }
    T query_range(int s, int t) {
        if (s >= t) return op.identity();
        node_t *root = &pool.front();
        if (s < root->b || t > root->e) throw 0;
        return query_range_inner(root, s, t);
    }
    T query_include(int s, int t) {
        return query_range(s, t + 1);
    }
private:
    T query_range_inner(node_t *cur, int s, int t) {
        if (!cur) return op.identity();
        // always overlap
        // inside
        if (s <= cur->b && t >= cur->e)
            return cur->val;
        // partial overlap
        else {
            T ret = op.identity();
            int m = (cur->b + cur->e) / 2;
            if (s < m) ret = op(ret, query_range_inner(cur->left, s, t));
            if (t > m) ret = op(ret, query_range_inner(cur->right, s, t));
            return ret;
        }
    }
    void increase_element_inner(node_t *cur, int p, const T &n) {
        cur->val = op(cur->val, n);

        if (cur->e - cur->b == 1) return;
        int m = (cur->b + cur->e) / 2;
        if (p < m)
            increase_element_inner(get_left(cur, m), p, n);
        else
            increase_element_inner(get_right(cur, m), p, n);
    }
    void update_element_inner(node_t *cur, int p, const T &n) {
        if (cur->e - cur->b == 1) {
            cur->val = n;
            return;
        }
        int m = (cur->b + cur->e) / 2;
        if (p < m)
            update_element_inner(get_left(cur, m), p, n);
        else
            update_element_inner(get_right(cur, m), p, n);

        // child value may change, recalculate aggregated value
        cur->val = op(cur->left ? cur->left->val : op.identity(),
                      cur->right ? cur->right->val : op.identity());
    }
};
template <typename T, typename Op>
struct implicit_segtree<T, Op, false>
{
    // Flavour II : query element, increase range (also increase element by operator[])
    //          Op must be commutative
    //          value(element) = sum{j}{vec[(element+offset)/2^j]} (aggregate all values above it)
    COMMON_CODE
    T query_element(int p) {
        node_t *root = &pool.front();
        if (p < root->b || p >= root->e) throw 0;
        return query_element_inner(root, p);
    }
    void increase_range(int s, int t, const T &inc) {
        if (s >= t) return;
        node_t *root = &pool.front();
        if (s < root->b || t > root->e) throw 0;
        return increase_range_inner(root, s, t, inc);
    }
    void increase_include(int s, int t, const T &inc) {
        increase_range(s, t + 1, inc);
    }
private:
    T query_element_inner(node_t *cur, int p) {
        if (!cur) return op.identity();
        // always overlap
        if (cur->e - cur->b == 1) return cur->val;

        int m = (cur->b + cur->e) / 2;
        if (p < m)
            return op(cur->val, query_element_inner(cur->left, p));
        else
            return op(cur->val, query_element_inner(cur->right, p));
    }
    void increase_range_inner(node_t *cur, int s, int t, const T &inc) {
        // always overlap
        // inside
        if (s <= cur->b && t >= cur->e)
            cur->val = op(inc, cur->val);
        // partial overlap
        else {
            int m = (cur->b + cur->e) / 2;
            if (s < m) increase_range_inner(get_left(cur, m), s, t, inc);
            if (t > m) increase_range_inner(get_right(cur, m), s, t, inc);
        }
    }
};
#undef COMMON_CODE
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_IMPLICIT_SEGTREE_H_ */
