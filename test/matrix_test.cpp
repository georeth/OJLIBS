#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <ojlibs/matrix.hpp>
#include <ojlibs/power.hpp>
#include <ojlibs/algebra/modular.hpp>
using namespace std;

using Alg = ojlibs::base_algebra<double>;
typedef ojlibs::matrix<Alg> mat_t;

template <typename Alg>
void print(const ojlibs::matrix<Alg> &m) {
    for (int i = 0; i < m.r(); ++i) {
        for (int j = 0; j < m.c(); ++j)
            cout << m(i, j) << " ";
        cout << endl;
    }
    cout << endl;
}

TEST(OP, MM) {
    mat_t m(3, 3);
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

        mat_t LU, PA(M, M);
        bool pos;
        auto [p, L, U, pivots] = PLU_decompose(A);
        ASSERT_EQ(pivots.size(), M);
        LU = L * U;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                PA(i, j) = A(p[i], j);
            }
        }

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j) {
                ASSERT_GE(1e-6, abs(PA(i, j) - LU(i, j)));
            }
    }
}

TEST(PLU, random_mod_singular_and_solve) {
    constexpr int MOD = 1000000009;
    using Alg = ojlibs::modular_algebra<MOD>;
    using mat_t = ojlibs::matrix<Alg>;

    const int M_MAX = 50;
    const int TEST_SIZE = 100;
    std::uniform_int_distribution<int> dis_m(1, M_MAX);
    std::uniform_int_distribution<int> dis(0, MOD - 1);
    std::mt19937 gen;

    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
        int M = dis_m(gen);
        int N = dis_m(gen);
        // make sure small rank is tested
        int rank = nr_test % (std::min(M, N) + 1);

        mat_t B(M, rank), C(rank, N);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < rank; ++j)
                B(i, j) = dis(gen);
        for (int i = 0; i < rank; ++i)
            for (int j = 0; j < N; ++j)
                C(i, j) = dis(gen);
        mat_t A = B * C; // rank(A) <= rank

        mat_t LU, PA(M, N);
        bool pos;
        auto info = ojlibs::PLU_decompose(A);
        auto &&[p, L, U, pivots] = info;

        ASSERT_EQ(p.size(), M);
        ASSERT_EQ(L.r(), M);
        ASSERT_EQ(L.c(), M);
        ASSERT_EQ(U.r(), M);
        ASSERT_EQ(U.c(), N);
        ASSERT_TRUE(pivots.size() <= rank);
        rank = pivots.size();

        // check PA=LU
        LU = L * U;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                PA(i, j) = A(p[i], j);
            }
        }
        if (PA != LU) {
            cout << "ps: " << pivots.size() << "\n";
            cout << "A:\n";
            print(A);
            cout << "PA:\n";
            print(PA);
            cout << "L:\n";
            print(L);
            cout << "U:\n";
            print(U);
            cout << "LU:\n";
            print(LU);
        }
        ASSERT_EQ(PA, LU);

        // solve Ax = y
        auto null_space = ojlibs::PLU_solve_general(info);
        if (!(A * null_space == mat_t(M, N - rank))) {
            cout << "ps: " << pivots.size() << "\n";
            cout << "A:\n";
            print(A);
            cout << "PA:\n";
            print(PA);
            cout << "L:\n";
            print(L);
            cout << "U:\n";
            print(U);
            cout << "X:\n";
            print(null_space);
            cout << "AX:\n";
            print(A * null_space);
        }
        ASSERT_EQ(A * null_space, mat_t(M, N - rank));

        mat_t x(N, 1);
        for (int i = 0; i < N; ++i)
            x(i, 0) = dis(gen);

        mat_t y = A * x;

        auto [x_2, valid] = ojlibs::PLU_solve_special(info, y);
        ASSERT_TRUE(valid);

        if (A * x_2 != y) {
            cout << "ps: " << pivots.size() << "\n";
            cout << "A:\n";
            print(A);
            cout << "PA:\n";
            print(PA);
            cout << "L:\n";
            print(L);
            cout << "U:\n";
            print(U);
            cout << "x:\n";
            print(x);
            cout << "y = A x:\n";
            print(A * x);
            cout << "x_2:\n";
            print(x_2);
            cout << "A x_2:\n";
            print(A * x_2);
        }

        ASSERT_EQ(A * x_2, y);
    }

}

TEST(INVERSE, random_mod) {
    constexpr int MOD = 1000000009;
    using Alg = ojlibs::modular_algebra<MOD>;
    using mat_t = ojlibs::matrix<Alg>;

    const int M = 50;
    const int TEST_SIZE = 20;
    std::uniform_int_distribution<int> dis(0, MOD - 1);
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
