#ifndef OJLIBS_INC_ALGEBRA_MODULAR_H_
#define OJLIBS_INC_ALGEBRA_MODULAR_H_

#include <cassert>
#include <ojlibs/arith/arith.hpp>
#include <ojlibs/arith/prime_list.hpp>
#include <ojlibs/algebra/base.hpp>

namespace ojlibs { // TO_BE_REMOVED

// taken from AtCoder Library (CC0)
// if p is not prime, return 0 even if primitive root exists
constexpr int primitive_root_constexpr(int p) {
    if (p == 2) return 1;
    if (!is_prime_slow(p)) return 0;
    int phi = p - 1;
    int m = phi;
    int divs[21]{}, cnt = 0;
    for (int d = 2; int64_t(d) * d <= m; ++d) if (m % d == 0) {
        divs[cnt++] = d;
        m /= d;
        while (m % d == 0)
            m /= d;
    }
    if (m != 1) divs[cnt++] = m;

    for (int g = 2; g < p - 1; ++g) {
        bool ok = true;
        for (int i = 0; i < cnt; ++i)
            if (pow_mod(g, phi / divs[i], p) == 1) {
                ok = false;
                break;
            }
        if (ok) return g;
    }
    // there should be a assertion that control cannot not reach here.
    // however, some compiler (e.g. GCC < 9) complains about assert(false) / throw 0,
    // even if it's not evaluated.
    return -1; // we have to return a value to suppress warning
}

template <int MOD = 1000000007>
struct modular_algebra {
    // only for int
    using T = int;
    using type = T;
    static T from(const T &v) { return mod_mod(v, MOD); }
    static T zero() { return T(); }
    static T one() { return 1; }
    static T negate(const T &p) { return subx_mod(zero(), p, MOD); }
    static T plus(const T &p1, const T &p2) { return addx_mod(p1, p2, MOD); }
    static T minus(const T &p1, const T &p2) { return subx_mod(p1, p2, MOD); }
    static T multiplies(const T &p1, const T &p2) { return mul_mod(p1, p2, MOD); }

    static T inverse(const T &p) { return inv_mod(p, MOD); }
    static T divides(const T &p1, const T &p2) { return div_mod(p1, p2, MOD); }

    static T norm(const T &p) { return p; }

    static constexpr T primitive_root = primitive_root_constexpr(MOD);

    static const std::vector<T> &unit_roots(int k, bool inv) {
        assert(primitive_root != 0);
        static std::vector<T> roots[2]; // FIXME: is static allow in this library?
        auto &v = roots[inv];
        if (v.empty()) {
            int m = MOD - 1;
            while (~m & 1) m >>= 1; // use c++20 countr_zero if possible

            int r = pow_mod(primitive_root, m, MOD);
            if (inv) r = inv_mod(r, MOD);
            while (r != 1) {
                v.push_back(r);
                r = mul_mod(r, r, MOD);
            }
            v.push_back(1);
            std::reverse(v.begin(), v.end());
        }
        assert(k < v.size());
        return v;
    }
};

template <typename T = int>
struct modular_algebra_dynamic {
    const int MOD;

    using type = T;
    modular_algebra_dynamic(int MOD): MOD(MOD) { }
    T from(const T &v) const { return mod_mod(v, MOD); }
    T zero() const { return T(); }
    T one() const { return 1; }
    T negate(const T &p) const { return subx_mod(zero(), p, MOD); }
    T plus(const T &p1, const T &p2) const { return addx_mod(p1, p2, MOD); }
    T minus(const T &p1, const T &p2) const { return subx_mod(p1, p2, MOD); }
    T multiplies(const T &p1, const T &p2) const { return mul_mod(p1, p2, MOD); }

    T inverse(const T &p) const { return inv_mod(p, MOD); }
    T divides(const T &p1, const T &p2) const { return div_mod(p1, p2, MOD); }
    T norm(const T &p) const { return p; }
};

template <int MOD=1000000007>
using mod_int = algebra_wrap<int, modular_algebra<MOD>>;

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ALGEBRA_MODULAR_H_ */
