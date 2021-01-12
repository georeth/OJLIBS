#include <gtest/gtest.h>
#include <cstdio>
#include <random>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

constexpr int MOD = 1000000007;
uniform_int_distribution dist(0, (1 << 10) - 1);

int dfs(int depth, int total, vector<int *> &numbers) {
    if (depth == total) {
        int res = 0;
        for (size_t idx = 0; idx < numbers.size(); ++idx) {
            int *p = numbers[idx];
            if (idx > 0) assert(p < numbers[idx - 1]);

            res = (13 * res + 133) % MOD;
            res += static_cast<int>(reinterpret_cast<uintptr_t>(p)) % 10000;
            res += *p;
            res %= MOD;
        }
        fprintf(stderr, "stderr stacksz: %zu bytes\n", reinterpret_cast<char *>(numbers.front()) - reinterpret_cast<char *>(numbers.back()));
        return res;
    }

    int k = dist(gen);
    numbers.push_back(&k);

    int res = dfs(depth + 1, total, numbers);
    return (res * res + 1997) % MOD;
}

TEST(BASIC, stk100) {
    vector<int *> vec;
    dfs(0, 100, vec);
}

TEST(BASIC, stk1000) {
    vector<int *> vec;
    dfs(0, 1000, vec);
}

TEST(BASIC, stk10000) {
    vector<int *> vec;
    dfs(0, 10000, vec);
}

TEST(BASIC, stk100000) {
    vector<int *> vec;
    dfs(0, 100000, vec);
}

TEST(BASIC, stk1000000) {
    vector<int *> vec;
    dfs(0, 1000000, vec);
}
