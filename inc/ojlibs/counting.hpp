#ifndef OJLIBS_INC_COUNTING_H_
#define OJLIBS_INC_COUNTING_H_

#include <ojlibs/arith.hpp>
namespace ojlibs { // TO_BE_REMOVED

// warning : mod is LARGE enough PRIME
struct factorial_list {
    std::vector<int> fact;
    int mod;
    factorial_list(int n, int mod) : fact(n + 1), mod(mod) {
        fact[0] = 1;
        for (int i = 1; i <= n; ++i)
            fact[i] = mul_mod(fact[i - 1], i, mod);
    }
    int operator()(int n) { return fact[n]; }
    int A(int n, int m) {
        return mul_mod(fact[n], inv_mod(fact[n - m], mod), mod);
    }
    int C(int n, int m) {
        if (m > n || n < 0 || m < 0) return 0;
        return mul_mod(A(n, m), inv_mod(fact[m], mod), mod);
    }
    int H(int n, int m) {
        return C(n + m - 1, m);
    }
};
// C(n, m) = C(n, n - m)
// Be cautious about overflow

// try Lucas if n is large, but r is small
// e.g C(10^15, 10^4, 1e9 + 7)
    
// use precompute C[][] if n and r are both small
// 30X faster than factorial list
template <int N, int Mod>
struct binomial_table {
    int arr[N][N];
    binomial_table() {
        arr[0][0] = 1;
        for (int i = 0; i < N; ++i) {
            arr[i][0] = 1;
            for (int j = 1; j < i; ++j)
                arr[i][j] = addx_mod(arr[i - 1][j], arr[i - 1][j - 1], Mod);
            arr[i][i] = 1;
        }
    }
    int C(int n, int m) {
        if (m > n || n < 0 || m < 0) return 0;
        return arr[n][m];
    }
    int H(int n, int m) {
        return C(n + m - 1, m);
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_COUNTING_H_ */
