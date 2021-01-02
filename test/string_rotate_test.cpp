#include <gtest/gtest.h>
#include <ojlibs/string/rotate.hpp>
#include <random>
#include <vector>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

int naive(const vector<int> &v_in) {
    size_t n = v_in.size();
    std::vector<int> v = v_in;
    v.insert(v.end(), v_in.begin(), v_in.end()); // doubling to avoid modular arith

    int ret = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (v[ret + j] != v[i + j]) {
                if (v[ret + j] < v[i + j])
                    ret = i;
                break;
            }
        }
    }
    return ret;
}

void check(const vector<int> &v) {
    int r1 = ojlibs::optimal_rotation<true>(v);
    int r2 = ojlibs::booth_algorithm<true>(v);
    int r3 = naive(v);

    vector<int> out1 = v;
    rotate(out1.begin(), out1.begin() + r1, out1.end());
    vector<int> out2 = v;
    rotate(out2.begin(), out2.begin() + r2, out2.end());
    vector<int> out3 = v;
    rotate(out3.begin(), out3.begin() + r3, out3.end());
    ASSERT_EQ(out1, out3);
    ASSERT_EQ(out2, out3);
}
void generate(vector<int> &v, int depth, int alpha) {
    if (v.size() == depth) {
        check(v);
    } else {
        for (int i = 0; i < alpha; ++i) {
            v.push_back(i);
            generate(v, depth, alpha);
            v.pop_back();
        }
    }
}

TEST(Random, small) {
    vector<int> v;
    generate(v, 10, 3);
}
