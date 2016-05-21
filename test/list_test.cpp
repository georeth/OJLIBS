#include <gtest/gtest.h>
#include <ojlibs/list.hpp>
#include <list>
namespace od = ojlibs::data_structure;
using namespace std;

struct int_node {
	struct od::list_head list;
	int val;
	int_node(int val): val(val){}
};

typedef od::list<int_node, &int_node::list> od_list;

TEST(Basic, Sequential) {
	od_list my_list;

	for (int i = 0; i < 100; i++) {
		int_node *node = new int_node(i);
		my_list.insert_back(node);
	}

	{
		auto iter = my_list.front_iter();
		for (int i = 0; i < 100; i++) {
			EXPECT_EQ(i, iter->val);
			++iter;
		}
	}
	{
		auto iter = my_list.front_iter();
		for (int i = 0; i < 100; i++) {
			my_list.erase(iter++);
		}
	}
}
