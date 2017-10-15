#include <vector>
#include <random>
#include <gtest/gtest.h>
#include <boost/multiprecision/gmp.hpp>
using namespace std;
namespace bm = boost::multiprecision;

template<typename F>
void digit_dc_inner(const vector<int> &num, int base, vector<int> &cur, int d, F &f) { // enumerate all [1, num] with same length
    if (d == (int)num.size()) {
        f(num, base, (int)num.size());
        return;
    }
    for (int i = 0; i < num[d]; ++i) {
        if (d == 0 && i == 0) continue;

        cur.push_back(i);
        f(cur, base, (int)num.size());
        cur.pop_back();
    }

    cur.push_back(num[d]);
    digit_dc_inner(num, base, cur, d + 1, f);
    cur.pop_back();
}

template<typename F>
void digit_dc(const vector<int> &num, int base, F &f) { // enumerate all [1, num]
    // shorter than num
    for (int len = 1; len < (int)num.size(); ++len) {
        for (int i = 1; i < base; ++i) {
            vector<int> cur = {i};
            f(cur, base, len);
        }
    }
    // the same length with num
    vector<int> cur;
    digit_dc_inner<F>(num, base, cur, 0, f);
}

std::mt19937 gen;
std::uniform_int_distribution<> dist_base(2, 20);
std::uniform_int_distribution<> dist_length(1, 100);

vector<int> random_number(int length, int base) {
    std::uniform_int_distribution<> dist0(1, base - 1);
    std::uniform_int_distribution<> dist(0, base - 1);

    vector<int> v;
    for (int i = 0; i < length; ++i) {
        if (i == 0) v.push_back(dist0(gen));
        else v.push_back(dist(gen));
    }
    return v;
}
bm::mpz_int to_mpz(vector<int> num, int base) {
    bm::mpz_int z = 0;
    for (int i = 0; i < (int)num.size(); ++i)
        z = z * base + num[i];
    return z;
}

TEST(Random, Print) {
    struct printer {
        void operator()(const vector<int> &v, int base, int length) {
            for (int i = 0; i < (int)v.size(); ++i) {
                printf("%d", v[i]);
            }
            for (int i = (int)v.size(); i < length; ++i)
                printf("X");
            printf("\n");
        }
    };
    int base = 10;
    static const int TEST_SIZE = 3;

    for (int test_case = 0; test_case < TEST_SIZE; ++test_case) {
        int length = std::uniform_int_distribution<>(1, 10)(gen);
        vector<int> num = random_number(length, base);

        printer p;
        digit_dc(num, base, p);
    }
}

TEST(Random, Count) {
    struct counter {
        bm::mpz_int cnt = 0;
        void operator()(const vector<int> &v, int base, int length) {
            cnt += pow(bm::mpz_int(base), length - (int)v.size());
        }
    };

    static const int TEST_SIZE = 500;
    for (int test_case = 0; test_case < TEST_SIZE; ++test_case) {
        int base = dist_base(gen);
        int length = dist_length(gen);
        vector<int> num = random_number(length, base);

        bm::mpz_int expected = to_mpz(num, base);

        counter c;
        digit_dc(num, base, c);
        bm::mpz_int answer = c.cnt;

        EXPECT_EQ(expected, answer);
    }
}
TEST(Random, Sum) {
    struct summer {
        bm::mpz_int cnt = 0;
        void operator()(const vector<int> &v, int base, int length) {
            int left = length - (int)v.size();
            bm::mpz_int p = pow(bm::mpz_int(base), left);
            cnt += to_mpz(v, base) * p * p;
            cnt += p * (p - 1) / 2;
        }
    };

    static const int TEST_SIZE = 100;
    for (int test_case = 0; test_case < TEST_SIZE; ++test_case) {
        int base = dist_base(gen);
        int length = dist_length(gen);
        vector<int> num = random_number(length, base);
        bm::mpz_int z = to_mpz(num, base);
        bm::mpz_int expected = z * (z + 1) / 2;

        summer s;
        digit_dc(num, base, s);
        bm::mpz_int answer = s.cnt;

        EXPECT_EQ(expected, answer);
    }
}

