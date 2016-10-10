#ifndef OJLIBS_INC_INDEX_RANGE_H_
#define OJLIBS_INC_INDEX_RANGE_H_

#include <iterator>
#include <ojlibs/iter_range.hpp>

namespace ojlibs { // TO_BE_REMOVED

// this is not an intrusive container
// explicitly requires T::next field

template <typename T>
struct index_iter {
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef iter_range<index_iter> range_type;
    typedef typename std::forward_iterator_tag iterator_category;

    int idx;
    T *array;
    index_iter() { idx = -1; array = nullptr; }
    index_iter(int index, T *array) : idx(index), array(array) { }
    index_iter& operator++() {
        idx = array[idx].next_index;
        return *this;
    }
    index_iter operator++(int) const {
        index_iter that(*this);
        return ++that;
    }
    pointer operator->() const {
        return &array[idx];
    }
    reference operator*() const {
        return *operator->();
    }
    int index() const { return idx; }
    bool operator==(const index_iter& that) {
        return idx == that.idx;
    }
    bool operator!=(const index_iter& that) {
        return !(*this == that);
    }
    static range_type make_range(int start, T *array) {
        return {{start, array}, {-1, array}};
    }
};

} // ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_INDEX_RANGE_H_ */
