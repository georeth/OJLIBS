#ifndef OJLIBS_INC_LIST_H_
#define OJLIBS_INC_LIST_H_

#include <iterator>
#include "ojlibs/util.hpp"

namespace ojlibs {
namespace data_structure {

/// # CORE
struct list_head {
	struct list_head *prev, *next;
	list_head() { prev = next = this; }

	list_head(const list_head &that) = delete;
	list_head &operator=(const list_head &that) = delete;
};
static inline bool list_empty(struct list_head *list) {
	return list->next == list;
}
static inline void list_init(struct list_head *list) {
	list->next = list->prev = list;
}
static inline void list_link(struct list_head *prev, struct list_head *next) {
	prev->next = next;
	next->prev = prev;
}
static inline void list_unlink(struct list_head *node) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list_init(node);
}
static inline void list_insert(struct list_head *prev, struct list_head *node, struct list_head *next) {
	list_link(prev, node);
	list_link(node, next);
}
static inline void list_splice(struct list_head *dest_prev, struct list_head *src, struct list_head *dest_next) {
	list_link(dest_prev, src->next);
	list_link(src->prev, dest_next);
	list_init(src);
}

/// # Iterator
template <typename T, struct list_head T::*field,
	  typename BaseIter = std::iterator<std::bidirectional_iterator_tag, T> >
struct list_iterator : public BaseIter
{
	// note that typedefs of base class is not visible during
	// unqualified name lookup
	typedef typename std::iterator_traits<BaseIter> traits_type;
	typedef typename traits_type::pointer pointer;
	typedef typename traits_type::reference reference;

	list_head *cur;
	list_iterator() { cur = nullptr; }
	list_iterator(list_head *head) { cur = head; }
	list_iterator(T *value) { cur = &(value->*field); }
	list_iterator& operator++() {
		cur = cur->next;
		return *this;
	}
	list_iterator operator++(int) const {
		list_iterator that(*this);
		return ++that;
	}
	list_iterator& operator--() {
		cur = cur->prev;
		return *this;
	}
	list_iterator operator--(int) const {
		list_iterator that(*this);
		return --that;
	}
	pointer operator->() const {
		return member_to_parent(cur, field);
	}
	reference operator*() const {
		return *operator->();
	}
	bool operator==(const list_iterator& that) {
		return cur == that.cur;
	}
	bool operator!=(const list_iterator& that) {
		return !(*this == that);
	}
};
template <typename T, struct list_head T::*field>
struct list {
	struct list_head head;
	typedef list_iterator<T, field> iterator_type;

	list() { list_init(&head); }
	iterator_type front_iter() { return iterator_type(head.next); }
	iterator_type back_iter() { return iterator_type(head.prev); }
	iterator_type end() { return iterator_type(&head); }

	iterator_type insert_front(T *node) {
		list_insert(&head, &node->*field, head.next);
		return iterator_type(node);
	}
	iterator_type insert_back(T *node) {
		list_insert(head.prev, &(node->*field), &head);
		return iterator_type(node);
	}
	iterator_type insert_before(T *node, const iterator_type &iter) {
		struct list_head *cur = iter.cur;
		list_insert(cur->prev, &(node->*field), cur);
		return iterator_type(node);
	}
	iterator_type insert_after(T *node, const iterator_type &iter) {
		struct list_head *cur = iter.cur;
		list_insert(cur, &(node->*field), cur->next);
		return iterator_type(node);
	}
	// return next iterator
	iterator_type erase(const iterator_type &iter) {
		struct list_head *cur = iter.cur;
		iterator_type next = iter++;
		list_unlink(cur);
		return next;
	}
};

} // data_structure
} // ojlibs

#endif /* end of include guard: OJLIBS_INC_LIST_H_ */
