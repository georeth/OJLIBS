#ifndef OJLIBS_INC_SEGTREE_H_
#define OJLIBS_INC_SEGTREE_H_

#include <vector>
#include <type_traits>
#include <ojlibs/bit_trick.hpp>

namespace ojlibs { // TO_BE_REMOVED
namespace data_structure { // TO_BE_REMOVED


template <typename T, typename Derived>
struct segtree_traits_crtp {
    static void assoc_inplace_left(T &left, const T &right) {
        left = assoc(left, right);
    }
    static void assoc_inplace_right(T &right, const T &left) {
        right = assoc(left, right);
    }
    static void assoc_inplace(T &value, const T &inc) {
        value = assoc(value, inc);
    }
    static T assoc(const T &x, const T &y) {
        return Derived::assoc(x, y);
    }
    // In Derived
    //      assoc(x, y)
    //      commutative (true_type / false_type)
};

template <typename T>
struct plus_traits : segtree_traits_crtp<T, plus_traits<T>> {
    typedef std::true_type commutative;
    static T assoc(const T &x, const T &y) { return x + y; }
};

template <typename T, typename traits_type = plus_traits<T>, bool flavour_one = true>
struct segtree {
    std::vector<T> vec;
    int size; // size of last layer
    int offset;
    segtree(int n) {
        size = n;
        offset = ceil_pow2(n);
        vec.resize(size + offset + 1); // real size += 1
    }
    void rebuild() {
        for (int i = (offset + size - 1) / 2; i > 0; --i){
            vec[i] = traits_type::assoc(vec[i + i], vec[i + i + 1]);
        }
    }

    // Flavour I : update element, query range
    //          value(element) = vec[element+offset]
    //          vec[p] = assoc(vec[2p], vec[2p+1]) always holds

    void increase_element(int p, const T &n) {
        static_assert(flavour_one, "flavour_one expected"); // TO_BE_REMOVED
        static_assert(traits_type::commutative::value, // TO_BE_REMOVED
                "increase_element requires commutative, use update_element instead"); // TO_BE_REMOVED
        for (p += offset; p; p >>= 1)
            traits_type::assoc_inplace(vec[p], n);
    }
    void update_element(int p, const T &n) {
        static_assert(flavour_one, "flavour_one expected"); // TO_BE_REMOVED
        p += offset;
        vec[p] = n;
        while (p >>= 1)
            vec[p] = traits_type::assoc(vec[p + p], vec[p + p + 1]);
    }
    T query_range(int s, int t) {
        static_assert(flavour_one, "flavour_one expected"); // TO_BE_REMOVED
        return query_include(s, t - 1);
    }
    T query_include(int s, int t) {
        static_assert(flavour_one, "flavour_one expected"); // TO_BE_REMOVED
        if (s > t) return T();
        s += offset;
        t += offset;
        T left_acc = vec[s];
        if (s == t) return left_acc;
        T right_acc = vec[t];
        // query exclude ]s, t[ next
        for (; s^t^1; s>>=1, t>>=1) {
            if (~s & 1) traits_type::assoc_inplace_left(left_acc, vec[s^1]);
            if (t & 1) traits_type::assoc_inplace_right(right_acc, vec[t^1]);
        }
        return traits_type::assoc(left_acc, right_acc);
    }

    // Flavour II : query element, update range
    //          value(element) = sum{j}{vec[(element+offset)/2^j]}

    T query_element(int p){
        static_assert(!flavour_one, "flavour_two expected"); // TO_BE_REMOVED
        static_assert(traits_type::commutative::value, // TO_BE_REMOVED
                "increase_element requires commutative, use update_element instead"); // TO_BE_REMOVED
        T ret = T();
        for (p += offset; p; p >>= 1)
            traits_type::assoc_inplace(ret, vec[p]);
        return ret;
    }
    void increase_range(int s, int t, const T &inc) {
        static_assert(!flavour_one, "flavour_two expected"); // TO_BE_REMOVED
        static_assert(traits_type::commutative::value, // TO_BE_REMOVED
                "increase_element requires commutative, use update_element instead"); // TO_BE_REMOVED
        return increase_include(s, t - 1, inc);
    }
    void increase_include(int s, int t, const T &inc) {
        static_assert(!flavour_one, "flavour_two expected"); // TO_BE_REMOVED
        static_assert(traits_type::commutative::value, // TO_BE_REMOVED
                "increase_element requires commutative, use update_element instead"); // TO_BE_REMOVED
        if (s > t) return;
        s += offset;
        t += offset;
        traits_type::assoc_inplace(vec[s], inc);
        if (s == t) return;
        traits_type::assoc_inplace(vec[t], inc);
        // query exclude ]s, t[ next
        for (; s^t^1; s>>=1, t>>=1) {
            if (~s & 1) traits_type::assoc_inplace(vec[s^1], inc);
            if (t & 1) traits_type::assoc_inplace(vec[t^1], inc);
        }
        return;
    }
    // Flavour I  : set initial value for element
    // Flavour II : update single element (range length = 1)
    T &operator[](int i) {
        return vec[offset + i];
    }
};

} // data_structure TO_BE_REMOVED
} // ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SEGTREE_H_ */
