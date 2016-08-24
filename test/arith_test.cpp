#include <gtest/gtest.h>
#include <ojlibs/arithmatic.hpp>
#include <random>
#include <boost/multiprecision/gmp.hpp>
using namespace std;
using namespace ojlibs::arithmatic;
namespace bm = boost::multiprecision;

std::mt19937 gen;
void test_gcd(int a, int b) {
	int g, x, y;
	g = gcd(a, b);

	ASSERT_EQ(0, a % g);
	ASSERT_EQ(0, b % g);
	ASSERT_EQ(1, gcd(a / g, b / g));

	g = extended_gcd(a, b, x, y);
	ASSERT_LT(abs(x), abs(b));
	ASSERT_LT(abs(y), abs(a));
	ASSERT_EQ(0, a % g);
	ASSERT_EQ(0, b % g);
	ASSERT_EQ(1, gcd(a / g, b / g));

	int sum = (int64_t)a * x + (int64_t)b * y;
	ASSERT_EQ(g, sum);

	int64_t m = lcm<int64_t, int>(a, b);
	ASSERT_EQ(0, m % a);
	ASSERT_EQ(0, m % b);
	ASSERT_EQ(1, gcd<int>(m / a, m / b));
}

TEST(GCD, Random) {
	const int TEST_SIZE = 10000;
	std::uniform_int_distribution<> dis(-2000000000, 2000000000);
	for (int i = 0; i < TEST_SIZE; ++i) {
		int a = dis(gen);
		int b = dis(gen);

		if (a == 0 && b == 0)
			continue;
		test_gcd(a, b);
	}
}
TEST(PowMod, Random) {
	const int TEST_SIZE = 10000;
	std::uniform_int_distribution<> dis(0, 2000000000);
	std::uniform_int_distribution<> mdis(1, 2000000000);
	for (int i = 0; i < TEST_SIZE; ++i) {
		int a = dis(gen);
		int p = dis(gen);
		int m = mdis(gen);
		bm::mpz_int ga = a, gp = p, gm = m;
		ASSERT_EQ(pow_mod(a, p, m), powm(ga, p, gm));
	}
}

TEST(InvMod, Random) {
	const int TEST_SIZE = 10000;
	std::uniform_int_distribution<> dis(0, 2000000000);
	std::uniform_int_distribution<> mdis(1, 2000000000);
	for (int i = 0; i < TEST_SIZE; ++i) {
		int a = dis(gen);
		int m = mdis(gen);
		if (a % m == 0) continue;
		if (gcd(a, m) != 1) continue;

		int v = inv_mod(a, m);
		bm::mpz_int ga = a, gm = m, gv = v;
		ASSERT_LT(0, v);
		ASSERT_EQ(1, ga * gv % gm);
	}
}
TEST(CRT2, Random) {
	const int TEST_SIZE = 10000;
	std::uniform_int_distribution<> dis(-2000000000, 2000000000);
	std::uniform_int_distribution<> mdis(1, 2000000000);
	for (int i = 0; i < TEST_SIZE; ++i) {
		int a1 = dis(gen);
		int a2 = dis(gen);
		int m1 = mdis(gen);
		int m2 = mdis(gen);

		int g = gcd(m1, m2);
		if (((int64_t)a1 - a2) % g != 0)
			continue;

		int64_t s = crt2(a1, m1, a2, m2);
		bm::mpz_int gs = s;
		ASSERT_LT(abs(s), bm::mpz_int(m1) * m2);
		ASSERT_EQ(0, (gs - a1) % m1);
		if ((gs - a2) % m2 != 0) {
			printf("%lld = %d + k %d\n", s, a1, m1);
			printf("%lld = %d + k %d\n", s, a2, m2);
		}
		ASSERT_EQ(0, (gs - a2) % m2);
	}
}

TEST(DivPosR, Random) {
	const int TEST_SIZE = 10000;
	std::uniform_int_distribution<> dis(-2000000000, 2000000000);
	for (int i = 0; i < TEST_SIZE; ++i) {
		int a = dis(gen);
		int b = dis(gen);
		if (b == 0) continue;
		int r;
		int q = div_pos_r(a, b, r);

		if (a != b * q + r) {
			printf("a=%d b=%d q=%d r=%d\n",a,b,q,r);
			printf("bq = %d\n",b*q);
		}
		ASSERT_EQ(a, b * q + r);
		ASSERT_LE(0, r);
	}
}
