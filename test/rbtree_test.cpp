#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <gtest/gtest.h>

#include <ojlibs/rb_tree.hpp>
using namespace std;
using namespace ojlibs;

struct my_node {
	struct rb_head head;
	int v;
	my_node(int v) : v(v) {}
	my_node() {}
	bool operator <(const my_node &that) const {
		return v < that.v;
	}
};
static inline int key_func(const my_node &n) {
	return n.v;
}
typedef rb_tree<my_node, &my_node::head> my_tree;
typedef rb_tree<my_node, &my_node::head, int, int(*)(const my_node &)> my_key_tree;

bool check_rbtree(struct rb_head *root, pair<int, bool> &info) {
	if (!root) {
		info.first = 0;
		info.second = 1;
		return true;
	}
	pair<int, bool> left, right;
	EXPECT_EQ(true, check_rbtree(root->left, left));
	EXPECT_EQ(true, check_rbtree(root->right, right));
	EXPECT_EQ(false, root->left && root->left->parent != root);
	EXPECT_EQ(false, root->right && root->right->parent != root);
	EXPECT_EQ(left.first, right.first);
	if (root->color == 0) {
		EXPECT_EQ(false, left.second == 0 || right.second == 0);
	}
	info.second = (root->color);
	info.first = left.first + (root->color ? 1 : 0);
	return true;
}
void print_rbtree(struct rb_head *root, int depth = 0) {
	if (!root) return;
	my_tree::iterator_type it(root);
	printf("[%10d] %s %d\n", it->v, string(depth, '*').c_str(), root->color);
	print_rbtree(root->left, depth + 1);
	print_rbtree(root->right, depth + 1);
}

class CompareTest : public ::testing::Test {
public:
	std::set<int> s;
	my_tree t;
	void CheckAll() {
		auto tit = t.front_iter();
		for (auto it = s.begin(); it != s.end(); ++it, ++tit) {
			ASSERT_EQ(false, tit == t.end());
			ASSERT_EQ(*it, tit->v);
		}
		ASSERT_EQ(true, tit == t.end());
	}
protected:
	CompareTest() {}
	virtual void SetUp() override {
	}
	virtual void TearDown() override {
		CheckAll();
	}
};

TEST(Basic, Basic) {
	printf("offset = %ld\n", offsetof(struct my_node, v));
	const int SIZE = 1000;
	my_tree tree;
	for (int i = 0; i < SIZE; i++) {
		ASSERT_EQ(true, static_cast<bool>(tree.insert(new my_node(i))));
	}
	int j = 0;
	for (auto iter = tree.front_iter(); iter; ++iter) {
		ASSERT_EQ(j++, iter->v);
	}
	for (int i = 0; i < SIZE; i++) {
		ASSERT_EQ(false, static_cast<bool>(tree.insert(new my_node(i))));
	}
	pair<int, bool> info;
	EXPECT_EQ(true, check_rbtree(tree.root.head, info));
	// print_rbtree(tree.root.head);
	auto it = tree.front_iter();
	while (it) {
		// print_rbtree(tree.root.head);
		EXPECT_EQ(true, check_rbtree(tree.root.head, info));
		my_node *ptr = it.ptr();
		it = tree.erase(it);
		delete ptr;
	}
}

TEST_F(CompareTest, Random) {
	const int SIZE = 20000;
	for (int i = 0; i < SIZE; i++) {
		// printf("i = %d\n", i);
		int v = rand() % 1000;
		auto it = t.find(my_node(v));
		bool found = s.count(v);
		EXPECT_EQ(found, static_cast<bool>(it));
		if (found) {
			// printf("erase %p -> %d\n", &it->head, v);
			s.erase(v);
			t.erase(it);
		} else {
			s.insert(v);
			it = t.insert(new my_node(v));
			EXPECT_EQ(true, static_cast<bool>(it));
			// printf("insert %p -> %d\n", &it->head, v);
		}
		pair<int, bool> info;
		check_rbtree(t.root.head, info);
	}
}

TEST(Speed, StdSet) {
	const int SIZE = 100000;
	std::set<int> s;
	for (int i = 0; i < SIZE; i++) {
		int v = rand() % 1000;
		bool found = s.count(v);
		if (found) {
			s.erase(v);
		} else {
			s.insert(v);
		}
	}
}
TEST(Speed, RbSet) {
	std::allocator<my_node> alloc;
	const int SIZE = 100000;
	my_key_tree t(key_func);
	for (int i = 0; i < SIZE; i++) {
		int v = rand() % 1000;
		auto it = t.find(v);
		if (it) {
			t.erase(it);
			delete it.ptr();
		} else {
			t.insert(new my_node(v));
		}
	}
}
