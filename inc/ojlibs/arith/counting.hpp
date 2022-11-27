#ifndef OJLIBS_INC_ARITH_COUNTING_H_
#define OJLIBS_INC_ARITH_COUNTING_H_

#include <algorithm>
#include <ojlibs/arith/arith.hpp>
#include <ojlibs/algebra/base.hpp>
namespace ojlibs { // TO_BE_REMOVED

// warning : mod is LARGE enough PRIME
// O(n) build
// O(1) query
struct factorial_list {
    std::vector<int> fact;
    std::vector<int> inv;
    std::vector<int> inv_fact;
    int mod;
    factorial_list(int n, int mod) : mod(mod) {
        fact.resize(n + 1);
        inv.resize(n + 1);
        inv_fact.resize(n + 1);

        fact[0] = 1;
        for (int i = 1; i <= n; ++i)
            fact[i] = mul_mod(fact[i - 1], i, mod);

        // magic: O(n) build inv[]
        // (MOD / i) * i + (MOD % i) = 0 (mod MOD)
        // (MOD % i) = -i * (MOD / i)
        // inv[i] = -MOD / i * inv[MOD % i];
        inv[1] = 1;
        for (int i = 2; i <= n; ++i) // start from 2
            inv[i] = mod - mul_mod(mod / i, inv[mod % i], mod);

        inv_fact[0] = 1;
        for (int i = 1; i <= n; ++i)
            inv_fact[i] = mul_mod(inv_fact[i - 1], inv[i], mod);
    }
    int operator()(int n) { return fact[n]; }
    int A(int n, int m) {
        return mul_mod(fact[n], inv_fact[n - m], mod);
    }
    int C(int n, int m) {
        if (m > n || n < 0 || m < 0) return 0;
        return mul_mod(A(n, m), inv_fact[m], mod);
    }
    int H(int n, int m) {
        return C(n + m - 1, m);
    }
    int T(int r1, int r2) {
        return C(r1 + r2, r1);
    }
};
// C(n, m) = C(n, n - m)
// Be cautious about overflow

// try Lucas if n is large, but r is small
// e.g C(10^15, 10^4, 1e9 + 7)
template <typename Alg = base_algebra<int>>
typename Alg::type C_slow(int n, int m, Alg alg = Alg()) {
    if (m > n || n < 0 || m < 0) return 0;
    m = std::min(m, n - m);

    auto ret = alg.one();
    for (int i = 0; i < m; ++i) {
        ret = alg.divides(alg.multiplies(ret, alg.from(n - i)), alg.from(i + 1));
    }
    return ret;
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_ARITH_COUNTING_H_
