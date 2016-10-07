#include <gtest/gtest.h>
#include <ojlibs/list.hpp>
#include <list>
namespace od = ojlibs;
using namespace std;

struct int_node {
    struct od::list_head list;
    int val;
    int_node(int val): val(val){}
};

TEST(Basic, Sequential) {
    od::list_head list;
    for (int i = 0; i < 100; ++i)
        list.insert_prev((new int_node(i))->list);

    auto it = od::list_range<int_node, &int_node::list>(list).first;
    for (int i = 0; i < 100; ++i, ++it)
        ASSERT_EQ(i, it->val);

    it = od::list_range<int_node, &int_node::list>(list).first;
    for (int i = 0; i < 100; ++i)
        (it++)->list.unlink();
    ASSERT_TRUE(it->list.empty());
}

TEST(Basic, list_range) {
    od::list_head list;
    for (int i = 0; i < 10; ++i)
        list.insert_prev((new int_node(i))->list);
    for (int_node &n : od::list_range<int_node, &int_node::list>(list)) {
        cout << n.val << endl;
    }
}
