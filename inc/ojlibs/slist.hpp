#ifndef OJLIBS_INC_SLIST_H_
#define OJLIBS_INC_SLIST_H_

#include <iterator>
#include <ojlibs/iter_range.hpp>
#include <ojlibs/intrusive_util.hpp>

namespace ojlibs { // TO_BE_REMOVED
namespace data_structure { // TO_BE_REMOVED

struct slist_head {
    struct slist_head *next; // always consistency
    slist_head() { reset(); }
    slist_head(const slist_head &that) = delete;
    slist_head &operator=(const slist_head &that) = delete;

    bool last() { return !next; }
    void unlink_from(slist_head &prev) { prev.next = next; reset(); }
    void insert_next(struct slist_head &new_next) {
        new_next.next = next;
        next = &new_next;
    }

private:
    void reset() { next = nullptr; }
};

template <typename T, struct slist_head T::*field>
struct slist_iter
{
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef iter_range<slist_iter> range_type;
    typedef typename std::forward_iterator_tag iterator_category;

    slist_head *cur;
    slist_iter() { cur = nullptr; }
    slist_iter(slist_head &head) { cur = &head; }
    slist_iter(T *value) { cur = &(value->*field); }
    slist_iter& operator++() {
        cur = cur->next;
        return *this;
    }
    slist_iter operator++(int) const {
        slist_iter that(*this);
        return ++that;
    }
    pointer operator->() const {
        return member_to_parent(cur, field);
    }
    reference operator*() const {
        return *operator->();
    }
    bool operator==(const slist_iter& that) {
        return cur == that.cur;
    }
    bool operator!=(const slist_iter& that) {
        return !(*this == that);
    }
    static range_type make_range(slist_head &slist) {
        return {slist_iter(*slist.next), nullptr};
    }
};

template <typename T, struct slist_head T::*field>
iter_range<slist_iter<T, field>> slist_range(slist_head &slist) {
    return slist_iter<T, field>::make_range(slist);
};

template <>
struct intrusive_traits<slist_head> {
    using head_type = slist_head;
    template <typename T, head_type T::*field>
    using iter_type = slist_iter<T, field>;
};

} // data_structure TO_BE_REMOVED
} // ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SLIST_H_ */
