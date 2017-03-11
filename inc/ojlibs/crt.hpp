#ifndef OJLIBS_INC_CRT_H_
#define OJLIBS_INC_CRT_H_

#include <ojlibs/arith.hpp>

namespace ojlibs { // TO_BE_REMOVED

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

template <typename Int, typename TInt = int64_t>
TInt crtn(const std::vector<Int> &as, const std::vector<Int> &ms) {
    // Pre condition:
    //          pairwise coprime
    TInt r = 1;
    TInt m = 1;
    for (int i = 0; i < as.size(); ++i) {
        r = crt2<TInt, TInt>(r, m, as[i], ms[i]);
        m *= ms[i];
    }
    return r;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_CRT_H_ */
