#ifndef OJLIBS_INC_ARITHMATIC_H_
#define OJLIBS_INC_ARITHMATIC_H_

#include <vector>
#include <limits>
#include <type_traits>
#include <utility>

// Suggested Reading : (GTM 138 by Henri Cohen)A course in computational algebraic number theory
//
// TODO : use gcc arith extension like __builtin_add_overflow

namespace ojlibs {
namespace arithmatic {

// try binary gcd on large number
template <typename Int>
Int gcd(Int a, Int b) {
    while (a != 0) {
        b %= a;
        if (b == 0)
            return a;
        a %= b;
    }
    return b;
}

template <typename Int, typename RInt = Int>
Int lcm(Int a, Int b) {
    return a / gcd(a, b) * static_cast<RInt>(b);
}

template <typename Int, typename XInt>
Int extended_gcd(Int a, Int b, XInt &x, XInt &y) {
    static_assert(std::numeric_limits<XInt>::is_signed,
            "x, y must be signed");
    // pre condition : a != 0 || b != 0
    // post condition : a * x + b * y = g
    // (a * x) or (b * y) may overflow, but the condition
    //                 always holds.
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    /**
     * r = a - b * q
     * b * k1 + r * k2 = g
     * => a * k2 + b * (k1 - q * k2)
     *                 x = k2
     *                 y = k1 - q * k2 = k1 - q * x
     */
    Int g = extended_gcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}

template <typename Int, typename TInt = int64_t>
Int mul_mod(Int a, Int b, Int m) {
    TInt p = a;
    p = (p * b) % m;
    return p;
}

template <typename Int, typename PInt, typename TInt = int64_t>
Int pow_mod(Int a, PInt p, Int m) {
    Int r;
    a %= m;
    for (r = 1; p; p >>= 1) {
        if (p & 1) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
    }
    return r;
}

template <typename Int,
          typename XInt = typename std::make_signed<Int>::type>
Int inv_mod(Int a, Int m) {
    // a * x = gcd(a, m) (mod m)
    XInt x, y;
    extended_gcd(a, m, x, y);
    if (x < 0)
        return x + m;
    return x;
}

template <typename Int>
Int div_pos_r(Int a, Int b, Int &r) {
    // 3 / 2  = 1 .. 1 (ok)
    // 3 / -2 = -1 .. 1 (ok)
    // -3 / 2 = -1 .. -1 (not ok)
    // -3 / -2 = 1 .. -1 (not ok)
    // a = q b + r
    //   = (q + 1) b + (r - b)
    //   = (q - 1) b + (r + b)
    Int q = a / b;
    r = a % b;
    if (r < 0) {
        if (b < 0)
            q += 1, r -= b;
        else
            q -= 1, r += b;
    }
    return q;
}

template <typename Int, typename TInt = int64_t>
TInt crt2(Int a1, Int m1, Int a2, Int m2) {
    // Pre condition: consistency
    //                 for every (i, j)
    //                 ai = aj (mod gcd(mi, mj))
    // Post condition:
    //         x = a1 (mod m1)
    //         x = a2 (mod m2)
    //  |x| < m1 * m2
    //
    //  x >= 0 may not hold!!
    Int k1, k2;
    Int g = extended_gcd(m1, m2, k1, k2);

    Int r1, r2;
    a1 = div_pos_r(a1, g, r1);
    a2 = div_pos_r(a2, g, r2);
    if (r1 != r2)
        throw 0;
    m1 /= g; m2 /= g; // no remainder

    TInt m12 = static_cast<TInt>(m1) * m2;
    // m1 * k1 + m2 * k2 = 1
    // a2 * m1 * k1 + a1 * m2 * k2
    //   = a1 + (a2 - a1) * m1 * k1 = a1 (mod m1)
    //   = a2 + (a1 - a2) * m2 * k2 = a2 (mod m2)
    return (static_cast<TInt>(a2) * k1 % m2 * m1 +
            static_cast<TInt>(a1) * k2 % m1 * m2) % m12 * g + r1;
}

template <typename Int>
Int counting(Int L, Int R, Int x0, Int dx) {
    // return number of d, s.t.
    //                 L <= x0 + d * dx <= R
    Int tmp;
    return div_pos_r(R - x0, dx, tmp) - div_pos_r(L - 1 - x0, dx, tmp);
}

template <typename Int>
Int crtn(const std::vector<Int> &as, const std::vector<Int> &ms) {
    // Pre condition :
    //                 pairwise coprime
    Int r = 1;
    Int m = 1;
    for (int i = 0; i < as.size(); ++i) {
        r = crt2(r, m, as[i], ms[i]);
        m *= ms[i];
    }
    return r;
}

/*
template <typename Int>
Int gauss_factorial(Int n, Int m);
template <typename Int>
Int factorial_mod(Int n, Int m);
*/

template <typename Int>
Int factorial_exp(Int n, Int p) {
    Int r = 0;
    while (n) {
        n = n / p;
        r += n;
    }
    return r;
}

template <typename Int>
std::pair<Int, bool> log(Int n, Int b) {
    // log = 3.3
    // return (3, true)
    if (n <= 0)
        return {-1, true};

    Int ans = 0;
    bool frac = false;
    while (n >= b) {
        frac = frac || (n % b != 0);
        ans += 1;
        n /= b;
    }
    if (n != 1) frac = true;
    return {ans, frac};
}

} // namespace arithmatic
} // namespace ojlibs
#endif /* end of include guard: OJLIBS_INC_ARITHMATIC_H_ */
