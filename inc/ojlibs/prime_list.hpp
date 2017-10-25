#ifndef OJLIBS_INC_PRIME_LIST_H_
#define OJLIBS_INC_PRIME_LIST_H_

#include <algorithm>
#include <vector>
#include <cstdint>
#include <utility>
#include <ojlibs/arith.hpp>

// TODO: quadradic residue

namespace ojlibs { // TO_BE_REMOVED

struct prime_list {
    prime_list(int n) {
        size = std::max(n, 2);
        prime_factor.resize(size);
        for (int p = 2; p < size; ++p) if (!prime_factor[p]) {
            primes.push_back(p);
            for (int pp = p; pp < size; pp += p)
                prime_factor[pp] = p;
        }
    }
    bool is_prime(int n) {
        return n > 1 && prime_factor[n] == n;
    }
    int least_prime_factor(int n) {
        return prime_factor[n];
    }
    int size;
    std::vector<int> prime_factor; // least prime factor, support O(number of prime factor) factorize.
    std::vector<int> primes;
};

using prime_factor_t = std::vector<std::pair<int64_t, int>>; // prime, multiplicity

// size of largest prime factor P1(x) : Golomb–Dickman constant
// P1(x) ~ exp(0.624 * log x) = x ^ 0.624
// time complexity is O(sqrt(P1(x))) = x ^ 0.312
template <typename Int>
prime_factor_t factorize(Int x) {
    prime_factor_t ret;
    if (x <= 1) return ret;
    for (int p = 2; Int(p) * p <= x; ++p) {
        int m = 0;
        while (x % p == 0) ++m, x /= p;
        if (m) ret.push_back(std::make_pair(p, m));

    }
    if (x > 1) 
        ret.push_back(std::make_pair(x, 1)); // x may larger than INT_MAX
    return ret;
}

template <typename Int>
bool is_prime_slow(Int x) {
    if (x <= 1) return false;
    for (Int p = 2; p * p <= x; ++p) {
        while (x % p == 0) return false;
    }
    return true;
}

// x <= plist.size * plist.size
// O(sqrt(P1(x)) / ln P1(x))
template <typename Int>
prime_factor_t factorize(Int x, const prime_list &plist) {
    prime_factor_t ret;
    if (x <= 1) return ret;
    for (int p : plist.primes) {
        if (x < Int(p) * p) break;

        int m = 0;
        while (x % p == 0) ++m, x /= p;
        if (m) ret.push_back(std::make_pair(p, m));
    }
    if (x > 1) 
        ret.push_back(std::make_pair(x, 1));
    return ret;
}

// sum up: worst case for n ~ 1e8 (k = sqrt(n))
// method               precalc         query
// prime_factor:        0,              O(sqrt(n))        ~ 10000
// prime_factor_fast:   O(k log k),     O(sqrt(n) / ln x) ~ 1235
// by least prime:      O(n log n),     O(log2(n))        ~ 26

/*
strong probable-prime base
32bit: 2, 7, 61
64bit: 2, 325, 9375, 28178, 450775, 9780504, 1795265022
*/

// deterministic Miller-rabin
template <typename Int>
bool is_prime(Int n);
template <>
bool is_prime(int n) {
    if (n <= 3) {
        if (n <= 1) return false;
        return true;
    }
    if (n % 2 == 0) return false;

    // n - 1 = 2^s * d
    int d = n - 1, s = 0;
    while (d % 2 == 0)
        d /= 2, ++s;

    for (int b : {2, 7, 61}) {
        int x = pow_mod(b, d, n);
        // (x == 0) case is not considered in most materials,
        // because they assume n is large enough.
        if (x == 0 || x == 1 || x == n - 1)
            continue;

        bool found = false;
        for (int i = 0; i < s - 1; ++i) {
            x = mul_mod(x, x, n);
            if (x == 1) return false;
            if (x == n - 1) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_PRIME_LIST_H_ */
