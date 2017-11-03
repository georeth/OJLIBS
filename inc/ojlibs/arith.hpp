#ifndef OJLIBS_INC_ARITH_H_
#define OJLIBS_INC_ARITH_H_

#include <cstdint>
#include <vector>
#include <limits>
#include <type_traits>

namespace ojlibs { // TO_BE_REMOVED

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

template <typename Int>
Int mul_mod(Int a, Int b, Int m) {
    typedef int64_t TInt;
    static_assert(2 * std::numeric_limits<Int>::digits <= std::numeric_limits<TInt>::digits,
            "multiplication will overflow!");
    TInt p = a;
    p = (p * b) % m;
    return (Int)p;
}

template <>
int64_t mul_mod<int64_t>(int64_t a, int64_t b, int64_t m) {
    int64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        r = (r + r) % m;
        if ((b >> i) & 1)
            r = (r + a) % m;
    }
    return r;
}

template <typename Int, typename PInt>
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
Int addx_mod(Int u, Int v, Int m) {
    Int r = u + v;
    if (r >= m) r -= m;
    return r;
}
template <typename Int>
Int subx_mod(Int u, Int v, Int m) {
    Int r = u - v;
    if (r < 0) r += m;
    return r;
}
template <typename Int>
Int div_mod(Int u, Int v, Int m) {
    // find x, s.t.
    //     u = v * x (mod m)
    // solution exist
    //     <=> gcd(v, m) | gcd(u, m)
    // g = gcd(v, m)   [do not use gcd(u, v)]
    // uu = u / g
    // vv = v / g
    // mm = m / g
    //     uu = vv * x (mod mm)
    Int g = gcd(v, m);
    assert(u % g == 0);
    Int mm = m / g;
    return mul_mod(u / g, inv_mod(v / g, mm), mm);
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

// TODO: gauss_factorial, factorial_mod.
//       log, factorial_exp

#ifdef OJLIBS_INC_SHORTHAND_H_ // TO_BE_REMOVED
#define INV_MOD(x)      inv_mod((x), MOD)
#define MUL_MOD(x, y)   mul_mod((x), (y), MOD)
#define MULM_MOD(x, y)  (x) = mul_mod((x), (y), MOD)
#define POW_MOD(x, y)   pow_mod((x), (y), MOD)
#define ADDX_MOD(x, y)  addx_mod((x), (y), MOD)
#define ADDM_MOD(x, y)  (x) = addx_mod((x), (y), MOD)
#define SUBX_MOD(x, y)  subx_mod((x), (y), MOD)
#define SUBM_MOD(x, y)  (x) = subx_mod((x), (y), MOD)
#endif /* OJLIBS_INC_SHORTHAND_H_ */ // TO_BE_REMOVED

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ARITH_H_ */
