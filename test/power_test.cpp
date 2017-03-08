#include <gtest/gtest.h>
#include <random>
#include <gmp.h>
#include <boost/multiprecision/gmp.hpp>
#include <ojlibs/power.hpp>
using namespace std;
using namespace ojlibs;
namespace bm = boost::multiprecision;

std::mt19937 gen;

void test_power(const bm::mpz_int &a, int b) {
	bm::mpz_int expect = pow(a, b);
	bm::mpz_int answer = power(a, b);

	EXPECT_EQ(expect, answer);
}

TEST(BASIC, SMALL) {
	test_power(12, 0);
	test_power(12, 1);
	test_power(12, 2);
}

TEST(BASIC, RANDOM) {
	gmp_randstate_t ran;
	gmp_randinit_default(ran);

	static const int TEST_GROUP = 1000;
	uniform_int_distribution<> dist(0, 10000);

	for (int i = 0; i < TEST_GROUP; ++i) {
		bm::mpz_int a;
		mpz_urandomb(a.backend().data(), ran, 15);

		int b = dist(gen);

		test_power(a, b);
	}
}
