#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <limits>
#include <ojlibs/prime_list.hpp>
using namespace std;

static const int MAX = 5000006;
static const int64_t IMAX = int64_t(MAX) * MAX - 10;
std::mt19937_64 gen;
std::uniform_int_distribution<> dist(0, MAX - 1);
std::uniform_int_distribution<int64_t> idist(0, IMAX);

bool naive_is_prime(int x) {
    if (x <= 1) return false;
    for (int p = 2; p < x; ++p)
        if (x % p == 0) return false;
    return true;
}
ojlibs::prime_list plist(MAX);
// static const int TEST_COUNT = 100000;
static const int TEST_COUNT = 1000;

TEST(BASIC, correct) {
    const int TEST_COUNT = 1000;
    for (int i = 0; i < TEST_COUNT; ++i) {
        int x = dist(gen);
        ASSERT_EQ(plist.is_prime(x), naive_is_prime(x));
    }
    ASSERT_EQ(plist.is_prime(9), naive_is_prime(9));
}

int64_t dummy; // force evaluation

TEST(BASIC, time_with_plist) {
    for (int i = 0; i < TEST_COUNT; ++i) {
        dummy += ojlibs::factorize(idist(gen), plist).size();
    }
}
TEST(BASIC, time_without_plist) {
    for (int i = 0; i < TEST_COUNT; ++i) {
        dummy += ojlibs::factorize(idist(gen)).size();
    }
}

TEST(BASIC, largest_prime) {
    int64_t sum, expected;
    for (int i = 0; i < 100; ++i) {
        int64_t x = idist(gen);
        sum += ojlibs::factorize(x).back().first;
        expected += pow(x, 0.624);
    }
    cout << "actual   : " << sum << endl;
    cout << "expected : " << expected << endl;
}

TEST(_, _) {
    cout << dummy << endl;
}
