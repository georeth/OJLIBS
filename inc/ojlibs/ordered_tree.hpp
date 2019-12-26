#ifndef OJLIBS_INC_ORDERED_TREE_H_
#define OJLIBS_INC_ORDERED_TREE_H_

#include <ext/pb_ds/assoc_container.hpp>
namespace ojlibs { // TO_BE_REMOVED
template <typename Key, typename Mapped, typename Cmp_Fn = std::less<Key>>
using ordered_tree = __gnu_pbds::tree<Key, Mapped, Cmp_Fn, __gnu_pbds::rb_tree_tag,
      __gnu_pbds::tree_order_statistics_node_update>;
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ORDERED_TREE_H_ */
