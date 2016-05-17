#include "ojlibs/list.hpp"
namespace ojlibs {
namespace data_structure {

bool list_empty(struct list_head *head) {
	return head->next == head;
}

void list_init(struct list_head *head) {
	head->prev = head;
	head->next = head;
}
void list_add(struct list_head *node, struct list_head *pos) {
	node->prev = pos;
	node->next = pos->next;
	node->prev->next = node;
	node->next->prev = node;
}
void list_del(struct list_head *node) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
	// left node uninitialized
}
void list_splice(struct list_head *from, struct list_head *head) {
	if (!list_empty(from)) {
		struct list_head *tail = head->next;
		struct list_head *first = from->next;
		struct list_head *last = from->prev;

		head->next = first;
		first->prev = head;
		tail->prev = last;
		last->next = tail;

		list_init(from);
	}
}

} // data_structure
} // ojlibs
