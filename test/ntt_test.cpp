#include <gtest/gtest.h>
#include <ojlibs/ntt.hpp>
#include <random>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

// corner case?
TEST(BASIC, identity) {
    int test_size = 1 << 4;
    int MOD = 998244353;
    int root = 3;
    int root_full = ojlibs::pow_mod(root, (MOD - 1) / test_size, MOD);
    assert((MOD - 1) % test_size == 0);

    for (int i = 1; i < test_size; ++i)
        assert(ojlibs::pow_mod(root_full, i, MOD) != 1);
    assert(ojlibs::pow_mod(root_full, test_size, MOD) == 1);

    vector<int> arr(test_size);
    for (int i = 0; i < arr.size(); ++i)
        arr[i] = uni(1, 12345)(gen);

    vector<int> arr2 = arr;
    ojlibs::NTT(arr2, MOD, root_full);
    ojlibs::NTT(arr2, MOD, root_full, true);
    for (int i = 0; i < test_size; ++i)
        printf("%d ", arr[i]);
    printf("\n");
    for (int i = 0; i < test_size; ++i)
        printf("%d ", arr2[i]);
    printf("\n");
    assert(arr == arr2);
}
