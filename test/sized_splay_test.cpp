#include <ojlibs/ds/sized_splay.hpp>
#include <gtest/gtest.h>
#include <random>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

struct int_node {
    int v;
    ojlibs::splay_head h;
};


TEST(BASIC, manully) {
    ojlibs::sized_splay<int_node, &int_node::h, int,
        ojlibs::key_accessor<int_node, int, &int_node::v>, std::less<int>> splay;
    vector<int_node> vec(10);
    for (int i = 0; i < 10; ++i) {
        vec[i].v = i * 2 % 10;
        splay.insert(&vec[i]);
    }
    for (int_node &n : splay) {
        printf("%d\n", n.v);
    }
    for (int i = 0; i < 3; ++i) {
        printf("begin %p\n", splay.begin().ptr());
        splay.erase(splay.begin());
    }
    for (int_node &n : splay) {
        printf("%d\n", n.v);
    }
    splay.erase(splay.find_nth(3));
    for (int_node &n : splay) {
        printf("%d\n", n.v);
    }

    printf("find 8 : %p\n", splay.find(8).cur);
    printf("find 7 : %p\n", splay.find(7).cur);
}
