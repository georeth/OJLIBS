#ifndef OJLIBS_INC_BINARY_OPERATOR_H_
#define OJLIBS_INC_BINARY_OPERATOR_H_
#include <algorithm>
#include <limits>
namespace ojlibs { // TO_BE_REMOVED
// a binary operator of semigroup with identity         TO_BE_REMOVED
// satisfies (a) closure (b) associative (c) identity   TO_BE_REMOVED
// implements:                                          TO_BE_REMOVED
//      T identity();                                   TO_BE_REMOVED
//      T operator()(T t1, T t2);                       TO_BE_REMOVED
template <typename T>
struct binary_plus {
    T identity() const { return T(); }
    T operator()(const T &t1, const T &t2) const { return t1 + t2; }
    T negate(const T &t) { return -t; }
};
template <typename T, T MOD>
struct modular_plus {
    T identity() const { return T(); }
    T operator()(const T &t1, const T &t2) const { return (t1 + t2) % MOD; }
    T negate(const T &t) { return (MOD - t) % MOD; }
};
template <typename T>
struct binary_xor {
    T identity() const { return T(); }
    T operator()(const T &t1, const T &t2) const { return t1 ^ t2; }
    T negate(const T &t) { return t; }
};
template <typename T>
struct binary_max {
    T identity() const { return std::numeric_limits<T>::min(); }
    T operator()(const T &t1, const T &t2) const { return std::max(t1, t2); }
};
template <typename T>
struct binary_min {
    T identity() const { return std::numeric_limits<T>::max(); }
    T operator()(const T &t1, const T &t2) const { return std::min(t1, t2); }
};
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_BINARY_OPERATOR_H_ */
