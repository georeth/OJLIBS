#ifndef OJLIBS_INC_MOBIUS_INVERSION_H_
#define OJLIBS_INC_MOBIUS_INVERSION_H_

#include <vector>
#include <ojlibs/binary_operator.hpp>

// Dirichlet convolution
//      (f * g)[i] = sum{d|i} f[d] * g[i / d]
// Constant function    1 = (1, 1, ..., 1, ...)
// Unit funciton        e = (1, 0, ..., 0, ...)
// Identity function    I = (1, 2, ..., k, ...)
// Mobius Inversion Formula
//      g = f * 1
//  <=> g[i] = sum{d|i} f[d]
//  Then
//      f = u * g
//  <=> f[i] = sum{d|i} u[d] * g[i / d]
//  u is Mobius function
//
//  Other relations:
//      EularPhi * 1 = I (EularPhi(n) is number of coprime numbers <= n)
//      I * 1 = Divisor (number of divisors)

// All range is inclusive, because it is in number theory.
// f[0] is ignored.

namespace ojlibs { // TO_BE_REMOVED

// given f
// return g = f * 1
template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> mobius_transform(const std::vector<Int> &f, Op op = Op()) {
    std::vector<Int> g = f;
    int n = (int)f.size() - 1;
    for (int i = n; i >= 1; --i)
        for (int j = i + i; j <= n; j += i)
            g[j] = op(g[j], g[i]);
    return g;
}

// mobius in {-1, 0, 1}
// if p^2 | x then mobius[x] = 0
// else mobius[x] = (-1) ^ number of prime factor of x
// O(n log n), do not use it unless incremental update.
std::vector<int> mobius_function(int n) {
    std::vector<int> mobius(n + 1, 0);
    mobius[1] = 1;
    for (int i = 1; i <= n; ++i)
        if (mobius[i])
            for (int j = i + i; j <= n; j += i)
                mobius[j] -= mobius[i];
    return mobius;
}

// given g
// return f = g * u
// O(n log n)
template <typename Int, typename Op = binary_plus<Int>>
std::vector<Int> mobius_inverse(const std::vector<Int> &g, Op op = Op()) {
    // we don't need a mobius[]
    // just reverse the operatios of mobius_transform
    std::vector<Int> f = g;
    int n = (int)g.size() - 1;
    for (int i = 1; i <= n; ++i)
        for (int j = i + i; j <= n; j += i)
            f[j] = op(f[j], op.negate(f[i]));
    return f;

}


} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MOBIUS_INVERSION_H_ */
