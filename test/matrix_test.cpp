#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <ojlibs/matrix.hpp>
#include <ojlibs/power.hpp>
#include <ojlibs/algebra/modular.hpp>
using namespace std;

using Alg = ojlibs::base_algebra<double>;
typedef ojlibs::matrix<Alg> mat_t;

template <typename T, typename Ring>
void print(const mat_t &m) {
    for (int i = 0; i < m.r(); ++i) {
        for (int j = 0; j < m.r(); ++j)
            cout << m(i, j) << " ";
        cout << endl;
    }
    cout << endl;
}

TEST(OP, MM) {
    mat_t m(3, 3, 1.0);
    mat_t m2 = m * m;
    m + m;
    m - m;
    m += m;
    m -= m;
    m += 3.0;
    3.0 += m;
    m = m * 3.0;
    m = 3.0 * m;
}

TEST(HELLO, power) {
    using mat_t = ojlibs::matrix<ojlibs::base_algebra<int>>;
    mat_t m(2, 2);
    m(1, 0) = 1;
    m(1, 1) = 1;
    m(0, 1) = 1;

    mat_t m1 = ojlibs::power(m, 6, mat_t::eye(2));
    mat_t m2 = m * m * m * m * m * m;

    assert(m1 == m2);
}

TEST(BASIC, operator) {
    using mat_t = ojlibs::matrix<ojlibs::base_algebra<int>>;
    auto m1 = mat_t::eye(3);
    auto m2 = mat_t::eye(3);
    m1 + 3;
    3 + m1;
    m1 - 3;
    3 - m1;
    m1 * 3;
    3 * m1;
    -m1;

    m1 + m2;
    m1 - m2;
    m1 * m2;

    m1 += 3;
    m1 -= 3;
    m1 *= 3;
    m1 += m2;
    m1 = m1 * m2;
}


TEST(MULT, random) {
    using mat_t = ojlibs::matrix<ojlibs::base_algebra<int>>;
    std::uniform_int_distribution<> dis(-100, 100);
    std::mt19937 gen;

    const int TEST_SIZE = 20;
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        mat_t m1(10, 200), m2(200, 100), m3(100, 20);
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 20; ++j) {
                m1(i, j) = dis(gen);
                m2(j, i) = dis(gen);
                m3(i, j) = dis(gen);
            }
        }
        ASSERT_EQ((m1 * m2) * m3, m1 * (m2 * m3));
    }
}

TEST(LU, random) {
    const int M = 50;
    const int TEST_SIZE = 200;
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;
    
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        mat_t A(M, M);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                A(i, j) = dis(gen);

        mat_t L, U, LU;
        ASSERT_EQ(0, LU_decompose(A, L, U));
        LU = L * U;
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                ASSERT_GE(1e-6, abs(A(i, j) - LU(i, j)));
    }

}
TEST(PLU, random) {
    const int M = 50;
    const int TEST_SIZE = 200;
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;
    
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        mat_t A(M, M);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                A(i, j) = dis(gen);

        mat_t L, U, LU, PA(M, M);
        vector<int> p;
        mat_t P;
        bool pos;
        ASSERT_EQ(0, PLU_decompose(A, p, L, U, pos));
        LU = L * U;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                PA(i, j) = A(p[i], j);
            }
        }
        P = ojlibs::permutation_matrix<Alg>(p);

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                ASSERT_GE(1e-6, abs(PA(i, j) - LU(i, j)));
        ASSERT_EQ(pos ? 1 : -1, determinant(P));
    }
}

TEST(INVERSE, random) {
    const int M = 50;
    const int TEST_SIZE = 200;
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;

    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        mat_t A(M, M);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                A(i, j) = dis(gen);

        mat_t R = inverse(A);
        mat_t I = A * R;

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                ASSERT_GE(1e-8, abs(I(i, j) - (i == j ? 1.0 : 0.0)));

    }
}

TEST(INVERSE, random_mod) {
    constexpr int MOD = 1000000009;
    using Alg = ojlibs::modular_algebra<MOD>;
    using mat_t = ojlibs::matrix<Alg>;

    const int M = 50;
    const int TEST_SIZE = 50;
    std::uniform_real_distribution<double> dis(0, MOD - 1);
    std::mt19937 gen;

    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        mat_t A(M, M);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                A(i, j) = dis(gen);

        mat_t R = inverse(A);
        mat_t I = A * R;

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                ASSERT_EQ(I(i, j), (i == j ? 1 : 0));

    }
}
