#include <gtest/gtest.h>
#include <ojlibs/prufer.hpp>
#include <ojlibs/disjoint_set.hpp>
#include <vector>
#include <utility>
#include <random>
#include <list>
#include <algorithm>
using namespace std;
std::mt19937 gen;
using uni = uniform_int_distribution<>;

bool same_tree(vector<pair<int, int>> &tree1, vector<pair<int, int>> &tree2) {
    for (auto &e : tree1)
        if (e.first > e.second) swap(e.first, e.second);
    sort(tree1.begin(), tree1.end());
    for (auto &e : tree2)
        if (e.first > e.second) swap(e.first, e.second);
    sort(tree2.begin(), tree2.end());

    return tree1 == tree2;
}

vector<pair<int, int>> random_tree(int size, int offset) {
    vector<pair<int, int>> tree;
    vector<int> roots;
    for (int i = 0; i < size; ++i)
        roots.push_back(i + offset);

    while (roots.size() > 1) {
        int x = uni(0, (int)roots.size() - 1)(gen);
        swap(roots[0], roots[x]);
        x = 0;
        int y = uni(1, (int)roots.size() - 1)(gen);
        swap(roots[1], roots[y]);
        y = 1;

        tree.push_back(make_pair(roots[x], roots[y]));
        // roots.erase(x);
        swap(roots[0], roots.back());
        roots.pop_back();
    }
    return tree;
}

TEST(BASIC, random) {
    for (int T = 0; T < 20; ++T) {
        uni dist_size(1000, 10000);
        uni dist_offset(0, 1000);
        vector<pair<int, int>> tree = random_tree(dist_size(gen), dist_offset(gen));

        vector<int> prufer = ojlibs::tree_to_prufer(tree);
        vector<pair<int, int>> tree2 = ojlibs::prufer_to_tree(prufer);

        ASSERT_TRUE(same_tree(tree, tree2));
    }
}
