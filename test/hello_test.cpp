#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <ojlibs/mat2.hpp>
using namespace std;

typedef matrix<double> mat_t;
struct base {
    void f() { cout << "meow" << endl; }
};
struct derived : private base {
    using base::f;
};

TEST(HELLO, hello) {
    array_backend<int> ab(3, 3, 3);
    // cout << ab[0][0] << endl;
    // cout << ab.column(2)[2] << endl;

    matrix<double> m(3, 3, 1.0);
    for (int i = 0; i < 3; ++i) {
        // cout << m[i][i] << endl;
    }
    matrix<double> m2 = m * m;
    m + m;
    m - m;
    m += m;
    m -= m;
    m += 3.0;
    3.0 += m;

    vector<double> v = {1, 2, 3};
    column_vector<double> u = m * column_vector<double>(v);
    column_vector<double> u2;
    u2 = u + 1.0;

    matrix<double, ring_traits, column_backend> mv(v);

    matrix<double> M(3, 3), L, U;
    LU_decompose(M, L, U);
}

TEST(BASIC, operator) {
    auto m1 = matrix<int>::eye(3);
    auto m2 = matrix<int>::eye(3);
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

template <typename T, template <typename> class Ring>
void print(const matrix<T, Ring> &m) {
    for (int i = 0; i < m.r(); ++i) {
	for (int j = 0; j < m.r(); ++j)
	    cout << m[i][j] << " ";
	cout << endl;
    }
}
TEST(BASIC, RCCR) {
    column_vector<int> c{1,2,3};
    row_vector<int> r{1,2,3,4};
    row_vector<int> r2{1,2,3};
    cout << r2 * c << endl;
    print(c * r);
}
TEST(MULT, random) {
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;

    const int TEST_SIZE = 20;
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
	matrix<int> m1(10, 200), m2(200, 100), m3(100, 20);
	for (int i = 0; i < 10; ++i) {
	    for (int j = 0; j < 20; ++j) {
		m1[i][j] = dis(gen);
		m2[j][i] = dis(gen);
		m3[i][j] = dis(gen);
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
	matrix<double> A(M, M);
	for (int i = 0; i < M; ++i)
	    for (int j = 0; j < M; ++j)
		A[i][j] = dis(gen);

	matrix<double> L, U, LU;
	ASSERT_EQ(0, LU_decompose(A, L, U));
	LU = L * U;
	for (int i = 0; i < M; ++i)
	    for (int j = 0; j < M; ++j)
		ASSERT_GE(1e-6, abs(A[i][j] - LU[i][j]));
    }

    /*
    print(A);
    print(L);
    print(U);
    print(LU);
    */
}
TEST(PLU, random) {
    const int M = 50;
    const int TEST_SIZE = 200;
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;
    
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
	matrix<double> A(M, M);
	for (int i = 0; i < M; ++i)
	    for (int j = 0; j < M; ++j)
		A[i][j] = dis(gen);

	matrix<double> L, U, LU, PA(M, M);
	vector<int> p;
	matrix<int> P;
	bool pos;
	ASSERT_EQ(0, PLU_decompose(A, p, L, U, pos));
	LU = L * U;
	for (int i = 0; i < M; ++i) {
	    PA[i] = A[p[i]];
	}
	P = permutation_matrix<int>(p);

	/*
	printf("p = \n");
	for (auto i : p) printf("%d ", i);
	printf("\n");

	printf("P = \n");
	print(P);
	printf("A = \n");
	print(A);
	printf("L = \n");
	print(L);
	printf("U = \n");
	print(U);
	printf("LU = \n");
	print(LU);
	printf("PA = \n");
	print(PA);
	*/

	for (int i = 0; i < M; ++i)
	    for (int j = 0; j < M; ++j)
		ASSERT_GE(1e-6, abs(PA[i][j] - LU[i][j]));
	ASSERT_EQ(pos ? 1 : -1, determinant(P));
    }
}

