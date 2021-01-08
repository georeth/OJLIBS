#ifndef OJLIBS_INC_SPARSE_COMMON_H_
#define OJLIBS_INC_SPARSE_COMMON_H_

#include <iterator>

namespace ojlibs { // TO_BE_REMOVED

// this is public interfact to identify edge
// use eid instead? so user has to restore i and j
struct sp_entry {
    int i, j, eid;
};

struct sp_out_entry {
    int j, eid;
};

template <typename T>
struct sp_iter_base {
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef const T &reference;
    typedef const T *pointer;
    typedef std::forward_iterator_tag iterator_category;
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_COMMON_H_ */
