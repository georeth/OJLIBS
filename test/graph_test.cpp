#include <gtest/gtest.h>
#include <ojlibs/graph.hpp>
#include <ojlibs/list.hpp>
#include <ojlibs/shorthand.hpp>
#include <ojlibs/int_range.hpp>

TEST(BASIC, basic) {
    graph<> g(10);
    g.add(1, 2);
    g.add(1, 3);
    g.add(1, 4);
    g.add(2, 3);
    g.add(3, 5);
    g.add(5, 2);

    for (auto i : range(g.size())) {
        for (auto &e : g.edges[i]) {
            cout << e.from << " -> " << e.to << endl;
        }
    }
}
