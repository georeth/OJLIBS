#ifndef OJLIBS_INC_INT_RANGE_H_
#define OJLIBS_INC_INT_RANGE_H_

#include <ojlibs/iter_range.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename Int = int>
struct int_iter {
    Int i;
    explicit int_iter(Int i = 0) : i(i) { }
    Int operator*() const { return i; }
    int_iter &operator++() { ++i; return *this; }
    int_iter &operator--() { --i; return *this; }
    int_iter operator+(Int step) const { return int_iter(i + step); }
    int_iter operator-(Int step) const { return int_iter(i - step); }
    Int operator-(const int_iter &that) const { return i - that.i; }
    int_iter &operator+=(Int step) { i += step; return *this; }
    int_iter &operator-=(Int step) { i -= step; return *this; }
    bool operator==(const int_iter &that) const { return  i == that.i; }
    bool operator!=(const int_iter &that) const { return !(*this == that); }
    Int operator[](Int n) { return i + n; }

    typedef Int difference_type;
    typedef Int value_type;
    typedef Int *pointer;
    typedef Int reference; // not a int container, never reference to int
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename Int = int>
using int_range = iter_range<int_iter<Int>>;

template <typename Int>
int_range<Int> range(Int b, Int e) { return {int_iter<Int>(b), int_iter<Int>(e > b ? e : b)}; }
template <typename Int>
int_range<Int> range(Int n) { return range(Int(), n); }
template <typename Int>
int_range<Int> nrange(Int n) { return range(Int(1), Int(n+1)); }
template <typename Int>
int_range<Int> inrange(Int b, Int e) { return range(b, Int(e + 1)); }

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_INT_RANGE_H_ */
