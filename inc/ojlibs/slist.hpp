#ifndef OJLIBS_INC_SLIST_H_
#define OJLIBS_INC_SLIST_H_

#include <iterator>

namespace ojlibs {
namespace data_structure {

/// # CORE
struct slist_head {
	struct slist_head *next;
};
extern void slist_init(struct slist_head *head);
extern void slist_add(struct slist_head *node, struct slist_head *pos);
extern void slist_del_after(struct slist_head *pos);
/// # HELPER
/// # CONTAINER

} // data_structure
} // ojlibs


#endif /* end of include guard: OJLIBS_INC_SLIST_H_ */
