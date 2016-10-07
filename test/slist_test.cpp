#include <gtest/gtest.h>
#include <ojlibs/slist.hpp>
#include <list>
namespace od = ojlibs;
using namespace std;

struct int_node {
    struct od::slist_head slist;
    int val;
    int_node(int val): val(val){}
};

TEST(Basic, Sequential) {
    od::slist_head slist;
    for (int i = 0; i < 100; ++i)
        slist.insert_next((new int_node(i))->slist);

    auto it = od::slist_range<int_node, &int_node::slist>(slist).first;
    for (int i = 0; i < 100; ++i, ++it)
        ASSERT_EQ(99 - i, it->val);
}

TEST(Basic, list_range) {
    od::slist_head slist;
    for (int i = 0; i < 10; ++i)
        slist.insert_next((new int_node(i))->slist);
    for (int_node &n : od::slist_range<int_node, &int_node::slist>(slist)) {
        cout << n.val << endl;
    }
}
