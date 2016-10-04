#include <gtest/gtest.h>
#include <ojlibs/graph.hpp>
#include <ojlibs/list.hpp>
#include <ojlibs/shorthand.hpp>

TEST(BASIC, basic) {
    graph<> g(10, 100);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 3);
    g.add_edge(3, 5);
    g.add_edge(5, 2);
}
