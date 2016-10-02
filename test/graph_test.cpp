#include <gtest/gtest.h>
#include <ojlibs/graph.hpp>
#include <ojlibs/list.hpp>
#include <ojlibs/shorthand.hpp>

TEST(BASIC, basic) {
    graph<> g(10, 100);
    graph<empty_type, list_head> g2(10, 100);
}
