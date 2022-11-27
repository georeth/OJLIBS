#ifndef OJLIBS_INC_LIST_H_
#define OJLIBS_INC_LIST_H_

#include <iterator>
#include <ojlibs/iter_range.hpp>
#include <ojlibs/intrusive_util.hpp>

namespace ojlibs { // TO_BE_REMOVED

struct list_head {
    struct list_head *prev, *next; // always consistency
    list_head() { reset(); }
    list_head(const list_head &that) = delete;
    list_head &operator=(const list_head &that) = delete;

    bool empty() { return next == this; }
    void unlink() { link(*prev, *next); reset(); }
    void insert_next(struct list_head &new_next) {
        link(new_next, *next);
        link(*this, new_next);
    }
    void insert_prev(struct list_head &new_prev) {
        link(*prev, new_prev);
        link(new_prev, *this);
    }
    static void link(struct list_head &prev, struct list_head &next) {
        prev.next = &next;
        next.prev = &prev;
    }
    void reset() { prev = next = this; }
};

template <typename T, struct list_head T::*field>
struct list_iter
{
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef iter_range<list_iter> range_type;
    typedef typename std::bidirectional_iterator_tag iterator_category;
    typedef typename std::ptrdiff_t difference_type; // must be signed
    typedef intrusive_helper<T, list_head, field> helper_type;

    list_head *cur;
    list_iter() { cur = nullptr; }
    list_iter(list_head &head) { cur = &head; }
    list_iter(T *value) { cur = &(value->*field); }
    list_iter& operator++() {
        cur = cur->next;
        return *this;
    }
    list_iter operator++(int) const {
        list_iter that(*this);
        return ++that;
    }
    list_iter& operator--() {
        cur = cur->prev;
        return *this;
    }
    list_iter operator--(int) const {
        list_iter that(*this);
        return --that;
    }
    pointer operator->() const {
        return helper_type::to_parent(cur);
    }
    reference operator*() const {
        return *operator->();
    }
    bool operator==(const list_iter& that) {
        return cur == that.cur;
    }
    bool operator!=(const list_iter& that) {
        return !(*this == that);
    }
    static range_type make_range(list_head &list) {
        return {list_iter(*list.next), list_iter(list)};
    }
};

template <typename T, struct list_head T::*field>
iter_range<list_iter<T, field>> list_range(list_head &list) {
    return list_iter<T, field>::make_range(list);
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_LIST_H_ */
