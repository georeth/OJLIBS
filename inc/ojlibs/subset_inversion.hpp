#ifndef OJLIBS_INC_SUBSET_INVERSION_H_
#define OJLIBS_INC_SUBSET_INVERSION_H_
// given f[x], x is a bitmask of a set.
// 0 <= x < 2^K
// 
// g[x] = sum{y subset x} f[y]
// h[x] = sum{z superset x} f[z]
// i[x] = sum{z} (-1)^(x intersect z) f[z]
//
// f[*] <-> g[*], h[*], i[*] can be compute in time O(K * 2^K)
//
// given x, h[*] => f[x] in O(2^K)
// given x, g[*] => f[x] in O(2^K)
//
// convolution:
// f[x] = sum{y and z = x} f1[y] * f2[z] -> g[x] = g1[x] * g2[x]
#include <vector>
#include <ojlibs/binary_operator.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> subset_transform(const std::vector<Int> &f, int k, Op op = Op()) {
    std::vector<Int> g = f;
    // g_i[x] = sum{ y[0,i[ subset x[0,i[, y[i,k[ = x[i,k[ } f[y]
    for (int i = 0; i < k; ++i) {
        int bit = 1 << i;
        for (int x = 0; x < (1 << k); ++x) if (x & bit)
            g[x] = op(g[x], g[x ^ bit]);
    }
    return g;
}
template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> subset_inverse(const std::vector<Int> &g, int k, Op op = Op()) {
    std::vector<Int> f = g;
    // f_i[x] = sum{ y[0,i[ = x[0,i[, y[i,k[ subset x[i,k[ } f[y]
    for (int i = 0; i < k; ++i) {
        int bit = 1 << i;
        for (int x = 0; x < (1 << k); ++x) if (x & bit)
            f[x] = op(f[x], op.negate(f[x ^ bit]));
    }
    return f;
}
template <typename Int, typename Op = binary_plus<Int>>
Int subset_inverse_single(const std::vector<Int> &g, int k, int x, Op op = Op()) {
    std::vector<bool> parity(1 << k);
    int ans = op.identity();
    // if __builtin_popcount is not O(1), we need DP on parity
    for (int y = 0; y < (1 << k); ++y) {
        if (y) parity[y] = !parity[y ^ (y & -y)];
        if ((x & y) == y) ans = op(ans, parity[y] ? op.negate(g[y]) : g[y]);
    }
    return parity[x] ? op.negate(ans) : ans;
}

template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> superset_transform(const std::vector<Int> &f, int k, Op op = Op()) {
    std::vector<Int> h = f;
    // h_i[x] = sum{ y[0,i[ superset x[0,i[, y[i,k[ = x[i,k[ } f[y]
    for (int i = 0; i < k; ++i) {
        int bit = 1 << i;
        for (int x = 0; x < (1 << k); ++x) if (~x & bit)
            h[x] = op(h[x], h[x ^ bit]);
    }
    return h;
}
template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> superset_inverse(const std::vector<Int> &h, int k, Op op = Op()) {
    std::vector<Int> f = h;
    // f_i[x] = sum{ y[0,i[ = x[0,i[, y[i,k[ superset x[i,k[ } f[y]
    for (int i = 0; i < k; ++i) {
        int bit = 1 << i;
        for (int x = 0; x < (1 << k); ++x) if (~x & bit)
            f[x] = op(f[x], op.negate(f[x ^ bit]));
    }
    return f;
}
template <typename Int, typename Op = binary_plus<Int>>
Int superset_inverse_single(const std::vector<Int> &h, int k, int x, Op op = Op()) {
    std::vector<bool> parity(1 << k);
    int ans = op.identity();
    // if __builtin_popcount is not O(1), we need DP on parity
    for (int y = 0; y < (1 << k); ++y) {
        if (y) parity[y] = !parity[y ^ (y & -y)];
        if ((x & y) == x) ans = op(ans, parity[y] ? op.negate(h[y]) : h[y]);
    }
    return parity[x] ? op.negate(ans) : ans;
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SUBSET_INVERSION_H_ */
