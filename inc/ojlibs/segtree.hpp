#ifndef OJLIBS_INC_SEGTREE_H_
#define OJLIBS_INC_SEGTREE_H_

#include <vector>
#include <type_traits>
#include "ojlibs/util.hpp"

namespace ojlibs {
namespace data_structure {

template <typename T>
struct segtree_traits {
    typedef std::true_type commutative;
    static void assoc_inplace_left(T &left, const T &right) {
        left = assoc(left, right);
    }
    static void assoc_inplace_right(const T &left, T &right) {
        right = assoc(left, right);
    }
    static T assoc(const T &x, const T &y) {
        return x + y;
    }
};

template <typename T, typename traits_type = segtree_traits<T> >
struct segtree {
    std::vector<T> vec;
    int size; // size of last layer
    segtree(int n) {
        ++n;
        size = ceil_pow2(n);
        vec.resize(size * 2);
    }
    void resize_and_clear(int n) {
        size = ceil_pow2(n);
        int old_cap = vec.size();
        int new_cap = size * 2;
        if (new_cap < old_cap) {
            vec.resize(new_cap);
            for (int i = 0; i < new_cap; ++i)
                vec[i] = T();
        } else {
            for (int i = 0; i < old_cap; ++i)
                vec[i] = T();
            vec.resize(new_cap);
        }
    }
    void resize_without_clear(int n) {
        size = ceil_pow2(n);
        int new_cap = size * 2;
        vec.resize(new_cap);
    }
    // if comm
    void increase(int p, const T &n){
        for (p += size; p; p >>= 1)
            traits_type::assoc_inplace(vec[p], n);
    }
    void update(int p, const T &n){
        p += size;
        vec[p] = n;
        while (p >>= 1)
            vec[p] = traits_type::assoc(vec[p + p], vec[p + p + 1]);
    }
    void rebuild(){
        for (int i = size - 1; i > 0; --i){
            vec[i] = traits_type::assoc(vec[i + i], vec[i + i + 1]);
        }
    }
    T query_range(int s, int t){
        query_include(s, t - 1);
    }
    T query_include(int s, int t) {
        if (s > t)
            return T();
        T left_acc = vec[s + size];
        if (s == t)
            return left_acc;
        T right_acc = vec[t + size];
        ++s; --t;
        for (s+=size-1, t+=size+1; s^t^1; s>>=1, t>>=1){
            if (~s & 1)
                traits_type::assoc_inplace_left(left_acc, vec[s^1]);
            if (t & 1)
                traits_type::assoc_inplace_right(vec[t^1], right_acc);
        }
        return traits_type::assoc(left_acc, right_acc);
    }
    T &operator[](int i) {
        return vec[size + i];
    }
};

} // data_structure
} // ojlibs
#endif /* end of include guard: OJLIBS_INC_SEGTREE_H_ */
