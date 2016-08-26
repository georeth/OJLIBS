#include <random>
#include <iostream>
#include <gtest/gtest.h>
#include <ojlibs/linear.hpp>
using namespace std;

template <typename T>
struct fake_ring : public ring_traits<T> {};

template <typename T>
struct binary_ring { };

template <>
struct binary_ring<int> {
    typedef int T;
    // addition -> xor
    // multiplication -> and

    // (a xor b) and c -> ok
    static T zero() { return 0; }
    static T negate(const T &p) { return p; }
    static T plus(const T &p1, const T &p2) { return p1 ^ p2; }
    static T minus(const T &p1, const T &p2) { return p1 ^ p2; }
    static T one() { return 1; }
    static T time(const T &p1, const T &p2) { return p1 && p2; }

    static T &plus_assign(T &p1, const T &p2) { return p1 ^= p2; }
    static T &minus_assign(T &p1, const T &p2) { return p1 ^= p2; }
    static T &time_assign(T &p1, const T &p2) { return p1 = p1 && p2; }

    // optional
    static T inverse(const T &p) { if (!p) throw 0; return true; }
    static T divide(const T &p1, const T &p2) {
	if (!p2) throw 0;
	return p1;
    }

    static T &divide_assign(T &p1, const T &p2) {
	p1 = divide(p1, p2);
	return p1;
    }

    // norm >= 0
    // norm == 0 <=> p == zero
    static double norm(const T &p) {
	if (!p) return 0;
	return 1;
    }
};

TEST(BASIC, construction) {
	matrix<int, fake_ring> m(3, 3);
	matrix<int, fake_ring> m2(3, 3);
	swap(m, m2);

	matrix<int> e = matrix<int>::scalar(3, 3);
	matrix<int> d = matrix<int>::diagonal({1, 3, 5});
	matrix<int> ed = e * d;

	for (int i = 0; i < 3; ++i) {
	    for (int j = 0; j < 3; ++j) {
		if (i != j)
		    ASSERT_EQ(0, ed[i][j]);
		else
		    ASSERT_EQ(3 + 6 * i, ed[i][j]);
	    }
	}
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
    m1 *= m2;

}
TEST(MULT, random) {
    std::uniform_real_distribution<double> dis(-100, 100);
    std::mt19937 gen;

    const int TEST_SIZE = 200;
    for (int nr_test = 0; nr_test < TEST_SIZE; ++nr_test) {
	matrix<int> m1(10, 20), m2(20, 10), m3(10, 20);
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

template <typename T, template <typename> class Ring>
void print(const matrix<T, Ring> &m) {
    for (int i = 0; i < m.r(); ++i) {
	for (int j = 0; j < m.r(); ++j)
	    cout << m[i][j] << " ";
	cout << endl;
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
