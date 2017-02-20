#ifndef OJLIBS_INC_PRIME_LIST_H_
#define OJLIBS_INC_PRIME_LIST_H_

#include <algorithm>
#include <vector>
#include <utility>

namespace ojlibs { // TO_BE_REMOVED

struct prime_list {
    prime_list(int n) {
        size = std::max(n, 2);
        is_prime.resize(size, true);
        is_prime[0] = false;
        is_prime[1] = false;
        for (int p = 2; p < size; ++p) if (is_prime[p]) {
            primes.push_back(p);
            for (int pp = p * 2; pp < size; pp += p)
                is_prime[pp] = false;
        }
    }
    int size;
    std::vector<bool> is_prime;
    std::vector<int> primes;
};

template <typename Int = int>
using prime_factor_t = std::vector<std::pair<Int, int>>; // prime, multiplicity

// size of largest prime factor P1(x) : Golomb–Dickman constant
// P1(x) ~ exp(0.624 * log x) = x ^ 0.624
// time complexity is O(sqrt(P1(x))) = x ^ 0.312
template <typename Int>
prime_factor_t<Int> prime_factor(Int x) {
    prime_factor_t<Int> ret;
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

static bool is_prime_slow(int x) {
    if (x <= 1) return false;
    for (int p = 2; p * p <= x; ++p) {
        while (x % p == 0) return false;
    }
    return true;
}

// x <= plist.size * plist.size
// O(sqrt(P1(x)) / ln x)
template <typename Int>
prime_factor_t<Int> prime_factor(Int x, const prime_list &plist) {
    prime_factor_t<Int> ret;
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
    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_PRIME_LIST_H_ */
