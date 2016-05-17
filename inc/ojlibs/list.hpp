#ifndef OJLIBS_INC_LIST_H_
#define OJLIBS_INC_LIST_H_

#include <iterator>

namespace ojlibs {
namespace data_structure {

/// # CORE
struct list_head {
	struct list_head *prev, *next;
};
extern bool list_empty(struct list_head *head);
extern void list_init(struct list_head *head);
// add node after pos
extern void list_add(struct list_head *node, struct list_head *pos);
extern void list_del(struct list_head *node);
/// # HELPER
/// # CONTAINER
template <typename T, size_t Offset,
	  typename BaseIter = std::iterator<std::bidirectional_iterator_tag, T> >
struct list_iterator : public BaseIter
{
	// note that typedefs of base class is not visible during
	// unqualified name lookup
	typedef std::iterator_traits<BaseIter> traits_type;

	list_head *cur;
	list_iterator() { cur = nullptr; }
	list_iterator(list_head *head) { cur = head; }
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
	typename traits_type::reference operator*() const {
		return *((T *)((char *)cur - Offset));
	}
	typename traits_type::pointer operator->() const {
		return ((T *)((char *)cur - Offset));
	}
	bool operator==(const list_iterator& that) {
		return cur == that.cur;
	}
	bool operator!=(const list_iterator& that) {
		return !(*this == that);
	}
};
template <typename T, size_t Offset>
struct list {
	typedef list_iterator<T, Offset> iterator_type;
	list_head head;

	list() { list_init(&head); }
	iterator_type begin() { return iterator_type(head.next); }
	iterator_type end() { return iterator_type(&head); }

	iterator_type insert_head(T *node) {
		list_head *cur = (list_head *)((char *)node + Offset);
		list_add(cur, &head);
		return iterator_type(cur);
	}
	iterator_type insert_tail(T *node) {
		list_head *cur = (list_head *)((char *)node + Offset);
		list_add(cur, head.prev);
		return iterator_type(cur);
	}
	iterator_type insert_before(T *node, const iterator_type &iter) {
		list_head *cur = (list_head *)((char *)node + Offset);
		list_add(cur, iter.cur->prev);
		return iterator_type(cur);
	}
	iterator_type insert_after(T *node, const iterator_type &iter) {
		list_head *cur = (list_head *)((char *)node + Offset);
		list_add(cur, iter.cur);
		return iterator_type(cur);
	}
	// return next iterator
	iterator_type erase(const iterator_type &iter) {
		iterator_type next = iter++;
		list_del(iter.cur);
		return next;
	}
};

} // data_structure
} // ojlibs

#endif /* end of include guard: OJLIBS_INC_LIST_H_ */
