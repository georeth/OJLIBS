#include <gtest/gtest.h>
#include <random>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;
uni dist_rand(100000);

vector<int> raw_data;
struct CTOR {
    CTOR() {
        static const int size = 1000000;
        for (int i = 0; i < size; ++i)
            raw_data.push_back(dist_rand(gen));
    }
} CTOR;

TEST(UNIQUE, by_map) {
    vector<int> result = raw_data;
    map<int, int> m;
    for (int v : result)
        m[v] = 0;

    int i = 0;
    for (auto &kv : m)
        kv.second = i++;

    int dummy = 0;

    for (int &v : result) {
        v = m[v];
        dummy += v;
    }
    printf("dummy %d\n", dummy);
}

TEST(UNIQUE, by_lowerbound) {
    vector<int> result = raw_data;

    vector<int> sorted = raw_data;
    sort(sorted.begin(), sorted.end());

    int dummy = 0;

    for (int &v : result) {
        v = (int)(lower_bound(sorted.begin(), sorted.end(), v) - sorted.begin());
        dummy += v;
    }
    printf("dummy %d\n", dummy);
}
