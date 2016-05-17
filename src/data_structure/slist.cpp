#include "ojlibs/slist.hpp"

namespace ojlibs {
namespace data_structure {

void slist_init(struct slist_head *head) {
	head->next = nullptr;
}
void slist_add(struct slist_head *node, struct slist_head *pos) {
	node->next = pos->next;
	pos->next = node;
}
void slist_del_after(struct slist_head *pos) {
	pos->next = pos->next->next;
}


} // data_structure
} // ojlibs
