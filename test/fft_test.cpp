#include <gtest/gtest.h>
#include <ojlibs/algebra/modular.hpp>
#include <ojlibs/algebra/complex.hpp>
#include <ojlibs/algebra/polynomial.hpp>
#include <ojlibs/util/shorthand.hpp>
#include <ojlibs/arith/counting.hpp>
#include <random>
/* =====  SOLUTION START  ===== */

using uni = std::uniform_int_distribution<>;
std::mt19937 gen;
using Alg = complex_algebra;

TEST(FFT, COMPLEX) {
    uni dist(0, 100);
    Alg alg;

    int n = 4;
    vector<Alg::type> P(1 << n), P2;
    for (int i = 0; i < (1 << n); ++i)
        P[i] = dist(gen);
    P2 = P;
    fft(P, n, false, alg);
    fft(P, n, true, alg);

    for (int i = 0; i < (1 << n); ++i)
        ASSERT_TRUE(abs(P[i] - P2[i]) < 1e-8);
}

TEST(FFT, MOD) {
    uni dist(0, 100);
    using Alg = modular_algebra<998244353>;
    Alg alg;

    int n = 4;
    vector<Alg::type> P(1 << n), P2;
    for (int i = 0; i < (1 << n); ++i)
        P[i] = dist(gen);
    P2 = P;
    fft(P, n, false, alg);
    fft(P, n, true, alg);

    ASSERT_EQ(P, P2);
}
