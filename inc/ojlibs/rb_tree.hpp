#ifndef OJLIBS_INC_RB_TREE_H_
#define OJLIBS_INC_RB_TREE_H_

namespace ojlibs {
namespace data_structure {

struct rb_head {
	struct rb_head *parent, *left, *right;
	int color;
};
struct rb_root {
	struct rb_head *head;
};

} // data_structure
} // ojlibs

#endif /* end of include guard: OJLIBS_INC_RB_TREE_H_ */
