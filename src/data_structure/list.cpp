#include "list.hpp"
namespace ojlibs {
namespace data_structure {

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

} // data_structure
} // ojlibs
