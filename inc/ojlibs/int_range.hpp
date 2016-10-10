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

typedef iter_range<int_iter<>> range_t;
range_t range(int b, int e) { return {int_iter<>(b), int_iter<>(e > b ? e : b)}; }
range_t range(int n) { return range(0, n); }
range_t nrange(int n) { return range(1, n+1); }
range_t inrange(int b, int e) { return range(b, e + 1); }

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_INT_RANGE_H_ */
