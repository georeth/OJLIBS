#ifndef OJLIBS_INC_ALGEBRA_BASE_H_
#define OJLIBS_INC_ALGEBRA_BASE_H_

#include <cmath>

namespace ojlibs { // TO_BE_REMOVED

// SFINAE
template <typename T>
struct base_algebra {
    using type = T;
    static T from(const T &v) { return v; } // may change representation
    static T zero() { return T(); }
    static T negate(const T &p) { return -p; }
    static T plus(const T &p1, const T &p2) { return p1 + p2; }
    static T minus(const T &p1, const T &p2) { return p1 - p2; }
    static T one() { return 1; }
    static T multiplies(const T &p1, const T &p2) { return p1 * p2; }

    static T inverse(const T &p) { return static_cast<T>(1) / p; }
    static T divides(const T &p1, const T &p2) { return p1 / p2; }

    static auto norm(const T &p1) { return std::abs(p1); }
};

template <typename T, typename A>
struct algebra_wrap : A {
    T v;

    // allow implicit conversion from T to algebra_wrap
    // another direction is forbidden
    algebra_wrap(const T &v, const A &a = A()) : A(a), v(this->from(v)) { }
    explicit algebra_wrap(const A &a = A()) : v(this->zero()) { }
    algebra_wrap &operator=(const T &p) { v = this->from(p); return *this; }

    algebra_wrap operator-() { return algebra_wrap(this->negate(v), *this); }
    algebra_wrap operator+(const algebra_wrap &that) { return algebra_wrap(this->plus(v, that.v), *this); }
    algebra_wrap operator-(const algebra_wrap &that) { return algebra_wrap(this->minus(v, that.v), *this); }
    algebra_wrap operator*(const algebra_wrap &that) { return algebra_wrap(this->multiplies(v, that.v), *this); }
    algebra_wrap operator/(const algebra_wrap &that) { return algebra_wrap(this->divides(v, that.v), *this); }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ALGEBRA_BASE_H_ */
