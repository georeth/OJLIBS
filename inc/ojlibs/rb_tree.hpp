#ifndef OJLIBS_INC_RB_TREE_H_
#define OJLIBS_INC_RB_TREE_H_

#include <utility>
#include <ojlibs/intrusive_util.hpp>

namespace ojlibs { // TO_BE_REMOVED

enum {
	RB_RED = 0,
	RB_BLACK = 1,
};

struct rb_head {
	struct rb_head *parent, *left, *right;
	int color;

	rb_head() {
		clear();
	}
	void clear() {
		parent = nullptr;
		left = right = nullptr;
		color = RB_RED;
	}
	bool empty() {
		return parent == this;
	}
};
struct rb_root {
	struct rb_head *head;
	bool empty() {
		return head;
	}
};

static inline struct rb_head *rb_first(struct rb_head *node) {
	if (!node)
		return nullptr;
	while (node->left)
		node = node->left;
	return node;
}
static inline struct rb_head *rb_last(struct rb_head *node) {
	if (!node)
		return nullptr;
	while (node->right)
		node = node->right;
	return node;
}
static inline struct rb_head *rb_prev(struct rb_head *node) {
	rb_head *ret;
	if (node->empty())
		return nullptr;

	if ((ret = rb_last(node->left)))
		return ret;
	while ((ret = node->parent) && node == ret->left)
		node = ret;
	return ret;
}
static inline struct rb_head *rb_next(struct rb_head *node) {
	rb_head *ret;
	if (node->empty())
		return nullptr;

	if ((ret = rb_first(node->right)))
		return ret;
	while ((ret = node->parent) && node == ret->right)
		node = ret;
	return ret;
}
static inline void rb_link_node(struct rb_head *node, struct rb_head *parent,
				struct rb_head **link) {
	node->clear();
	node->parent = parent;

	*link = node;
}
static inline void rb_change_child(struct rb_head *old_child, struct rb_head *new_child,
				   struct rb_head *parent, struct rb_root *root) {
	if (parent) {
		if (old_child == parent->left)
			parent->left = new_child;
		else
			parent->right = new_child;
	} else {
		root->head = new_child;
	}
	if (new_child)
		new_child->parent = parent;
}
static inline void rb_replace_node(struct rb_head *old_node, struct rb_head *new_node,
				   struct rb_root *root) {
		new_node->left = old_node->left;
		new_node->right = old_node->right;
		new_node->color = old_node->color;
		if (new_node->left)
			new_node->left->parent = new_node;
		if (new_node->right)
			new_node->right->parent = new_node;
		rb_change_child(old_node, new_node, old_node->parent, root);
}
static inline void rb_rotate_left(struct rb_head *old_root, struct rb_head *new_root,
				  struct rb_root *root) {
	/*       old             new
	 *       / \             / \
	 *      a  new    ->   old  c
	 *         / \         / \
	 *        x   c       a   x
	 */
	struct rb_head *x = new_root->left,
	               *pp = old_root->parent;

	old_root->right = x;
	if (x)
		x->parent = old_root;
	new_root->left = old_root;
	old_root->parent = new_root;

	rb_change_child(old_root, new_root, pp, root);
}
static inline void rb_rotate_right(struct rb_head *old_root, struct rb_head *new_root,
				   struct rb_root *root) {
	/*       new             old
	 *       / \             / \
	 *      a  old    <-   new  c
	 *         / \         / \
	 *        x   c       a   x
	 */
	struct rb_head *x = new_root->right,
	               *pp = old_root->parent;

	old_root->left = x;
	if (x)
		x->parent = old_root;
	new_root->right = old_root;
	old_root->parent = new_root;

	rb_change_child(old_root, new_root, pp, root);
}
static inline void rb_insert(struct rb_head *n, struct rb_root *root) {
	struct rb_head *p, *pp, *u;
	while (true) {
		p = n->parent;
		if (!p) {
			n->color = RB_BLACK;
			break;
		} else if (p->color == RB_BLACK) {
			break;
		}

		pp = p->parent;
		if (p == pp->left) {
			u = pp->right;
			if (u && u->color == RB_RED) {
				u->color = RB_BLACK;
				p->color = RB_BLACK;
				pp->color = RB_RED;
				n = pp;
				continue;
			}
			if (n == p->right) {
				rb_rotate_left(p, n, root);
				std::swap(n, p);
			}
			p->color = RB_BLACK;
			pp->color = RB_RED;
			rb_rotate_right(pp, p, root);
			break;
		} else { // p == pp->right
			u = pp->left;
			if (u && u->color == RB_RED) {
				u->color = RB_BLACK;
				p->color = RB_BLACK;
				pp->color = RB_RED;
				n = pp;
				continue;
			}
			if (n == p->left) {
				rb_rotate_right(p, n, root);
				std::swap(n, p);
			}
			p->color = RB_BLACK;
			pp->color = RB_RED;
			rb_rotate_left(pp, p, root);
			break;
		}

	}
}
static inline struct rb_head *rb_erase_inner(struct rb_head *n, struct rb_root *root) {
	struct rb_head *left = n->left,
		       *right = n->right,
		       *p = n->parent;
	if (!left && !right) {
		rb_change_child(n, nullptr, p, root);
		return n->color == RB_BLACK ? p : nullptr;
	} else if (!left) {
		rb_change_child(n, right, p, root);
		right->color = RB_BLACK;
		return nullptr;
	} else if (!right) {
		rb_change_child(n, left, p, root);
		left->color = RB_BLACK;
		return nullptr;
	} else {
		struct rb_head *succ, *succ_right, *rebalance = nullptr;
		// IDEA: erase succ(n), then swap(n, succ(n))
		// 1. ERASE
		if (!right->left) {
			succ = right;
			succ_right = succ->right;
			if (succ_right) {
				succ_right->color = RB_BLACK;
				succ_right->parent = n;
			} else {
				rebalance = (succ->color == RB_BLACK) ? succ : nullptr;
			}
			n->right = succ_right;
		} else {
			succ = rb_first(right);
			succ_right = succ->right;
			if (succ_right) {
				succ_right->color = RB_BLACK;
				succ_right->parent = succ->parent;
			} else {
				rebalance = (succ->color == RB_BLACK) ? succ->parent : nullptr;
			}
			succ->parent->left = succ_right;
		}
		// 2. swap
		rb_replace_node(n, succ, root);

		return rebalance;
	}
}
static inline void rb_erase_rebalance(struct rb_head *p, struct rb_root *root) {
	struct rb_head *n = nullptr;
	struct rb_head *sib, *sib_far, *sib_near;
	while (true) {
		// INPUT: p(not null), n
		if (n == p->left) {
			sib = p->right;
			if (sib->color == RB_RED) {
				p->color = RB_RED;
				sib->color = RB_BLACK;
				rb_rotate_left(p, sib, root);
				sib = p->right;
			}
			sib_far = sib->right;
			if (!sib_far || sib_far->color == RB_BLACK) {
				sib_near = sib->left;
				if (!sib_near || sib_near->color == RB_BLACK) {
					sib->color = RB_RED;
					if (p->color == RB_RED) {
						p->color = RB_BLACK;
					} else {
						n = p;
						p = n->parent;
						if (p)
							continue;
					}
					break;
				}
				sib->color = RB_RED;
				sib_near->color = RB_BLACK;
				rb_rotate_right(sib, sib_near, root);
				sib_far = sib;
				sib = sib_near;
			}
			sib->color = p->color;
			sib_far->color = RB_BLACK;
			p->color = RB_BLACK;
			rb_rotate_left(p, sib, root);
			break;
		} else { // n = p->right
			sib = p->left;
			if (sib->color == RB_RED) {
				p->color = RB_RED;
				sib->color = RB_BLACK;
				rb_rotate_right(p, sib, root);
				sib = p->left;
			}
			sib_far = sib->left;
			if (!sib_far || sib_far->color == RB_BLACK) {
				sib_near = sib->right;
				if (!sib_near || sib_near->color == RB_BLACK) {
					sib->color = RB_RED;
					if (p->color == RB_RED) {
						p->color = RB_BLACK;
					} else {
						n = p;
						p = n->parent;
						if (p)
							continue;
					}
					break;
				}
				sib->color = RB_RED;
				sib_near->color = RB_BLACK;
				rb_rotate_left(sib, sib_near, root);
				sib_far = sib;
				sib = sib_near;
			}
			sib->color = p->color;
			sib_far->color = RB_BLACK;
			p->color = RB_BLACK;
			rb_rotate_right(p, sib, root);
			break;
		}
	}
}
static inline void rb_erase(struct rb_head *n, struct rb_root *root) {
	struct rb_head *rebalance;
	rebalance = rb_erase_inner(n, root);
	// rebalance have a subtree which lack for a black node
	if (rebalance)
		rb_erase_rebalance(rebalance, root);
}

template <typename T, struct rb_head T::*field,
	  typename BaseIter = std::iterator<std::bidirectional_iterator_tag, T> >
struct rb_iterator : BaseIter {
	typedef typename std::iterator_traits<BaseIter> traits_type;
	typedef typename traits_type::pointer pointer;
	typedef typename traits_type::reference reference;

	struct rb_head *cur;
	rb_iterator() { cur = nullptr; }
	rb_iterator(rb_head *head) { cur = head; }
	rb_iterator(T *value) { cur = &(value->*field); }
	explicit operator bool() {
		return cur;
	}
	rb_iterator& operator++() {
		cur = rb_next(cur);
		return *this;
	}
	rb_iterator operator++(int) const {
		rb_iterator that(*this);
		return ++that;
	}
	rb_iterator& operator--() {
		cur = rb_prev(cur);
		return *this;
	}
	rb_iterator operator--(int) const {
		rb_iterator that(*this);
		return --that;
	}
	pointer ptr() const {
		return member_to_parent(cur, field);
	}
	pointer operator->() const {
		return ptr();
	}
	reference operator*() const {
		return *operator->();
	}
	bool operator==(const rb_iterator& that) {
		return cur == that.cur;
	}
	bool operator!=(const rb_iterator& that) {
		return !(*this == that);
	}
};

template <typename T, struct rb_head T::*field, typename Key = T,
	  typename KeyFunc = identity_accessor<T>, typename KeyCompar = std::less<Key> >
struct rb_tree {
	typedef rb_iterator<T, field> iterator_type;

	struct rb_root root;
	KeyCompar key_compar;
	KeyFunc key_func;

	rb_tree(KeyFunc func = KeyFunc(), KeyCompar compar = KeyCompar()) {
		root.head = nullptr;
		key_compar = compar;
		key_func = func;
	}
	iterator_type front_iter() { return iterator_type(rb_first(root.head)); }
	iterator_type back_iter() { return iterator_type(rb_last(root.head)); }
	iterator_type end() { return iterator_type(); }

	iterator_type find(const Key &key) {
		struct rb_head *head = root.head;
		T *cur;
		while (head) {
			cur = member_to_parent(head, field);
			auto &&nkey = key_func(*cur);
			if (key_compar(key, nkey))
				head = head->left;
			else if (key_compar(nkey, key))
				head = head->right;
			else
				return iterator_type(head);
		}
		return {};
	}
	iterator_type lower_bound(const Key &key) {
		rb_head *x = nullptr;
		rb_head *head = root.head;
		while (head) {
			T *cur = member_to_parent(head, field);
			if (key_compar(key_func(*cur), key)) {
				cur = cur->right;
			} else {
				x = cur;
				cur = cur->left;
			}
		}
		return iterator_type(x);
	}
	iterator_type insert(T *node) {
		struct rb_head **link = &(root.head), *parent = nullptr;
		struct rb_head *head = &(node->*field);
		auto &&key = key_func(*node);
		T *cur;

		while (*link) {
			cur = member_to_parent(*link, field);
			parent = *link;
			auto &&nkey = key_func(*cur);
			if (key_compar(key, nkey))
				link = &((*link)->left);
			else if (key_compar(nkey, key))
				link = &((*link)->right);
			else
				// return null iterator
				return iterator_type();
		}
		rb_link_node(head, parent, link);
		rb_insert(head, &root);
		return iterator_type(head);
	}
	// return next iterator
	iterator_type erase(const iterator_type &iter) {
		iterator_type next = iter++;
		rb_erase(iter.cur, &root);
		return next;
	}
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_RB_TREE_H_ */
