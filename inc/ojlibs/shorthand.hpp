// if you interested in the reference code here,
// see https://github.com/georeth/OJLIBS for more information
//
// Solution Author : OJLIBS_CODER_NAME
#ifndef OJLIBS_INC_SHORTHAND_H_
#define OJLIBS_INC_SHORTHAND_H_
#include <cstdio>
#include <cstring>      // memset
#include <cstdlib>      // malloc
#include <vector>
#include <map>
#include <set>
#include <tuple>        // pair, tuple
#include <iostream>
#include <algorithm>
#include <iterator>     // reverse_iterator
#include <ojlibs/iter_range.hpp>
using namespace std;    // use as solution header. name polution is fine.
namespace ojlibs { namespace data_structure {}} // TO_BE_REMOVED
using namespace ojlibs;   // TO_BE_REMOVED
using namespace ojlibs::data_structure;   // TO_BE_REMOVED

#ifdef LOCAL_JUDGE
#  define pr_debug(...) fprintf(stderr, __VA_ARGS__)
#else
#  define pr_debug(...)
#endif

struct int_iter {
    int i;
    explicit int_iter(int i = 0) : i(i) { }
    int operator*() const { return i; }
    int_iter &operator++() { ++i; return *this; }
    int_iter &operator--() { --i; return *this; }
    int_iter operator+(int step) const { return int_iter(i + step); }
    int_iter operator-(int step) const { return int_iter(i - step); }
    int operator-(const int_iter &that) const { return i - that.i; }
    int_iter &operator+=(int step) { i += step; return *this; }
    int_iter &operator-=(int step) { i -= step; return *this; }
    bool operator==(const int_iter &that) const { return  i == that.i; }
    bool operator!=(const int_iter &that) const { return !(*this == that); }

    typedef int difference_type;
    typedef int value_type;
    typedef int *pointer;
    typedef int reference; // not a int container, never reference to int
    typedef std::random_access_iterator_tag iterator_category;
};
struct range {
    int b, e;
    range(int bp, int ep) : b(bp), e(ep) { if (e < b) e = b; }
    range(int ep) : b(0), e(ep) { if (e < 0) e = 0; }
    int_iter begin() const { return int_iter(b); }
    int_iter end() const { return int_iter(e); }
};

typedef iter_range<int_iter> range_t;
range_t range(int b, int e) { return {int_iter(b), int_iter(e > b ? e : b)}; }
range_t range(int n) { return range(0, n); }
range_t nrange(int n) { return range(1, n+1); }
range_t inrange(int b, int e) { return range(b, e + 1); }

template <typename T>
T &chmax(T &m, const T &v) { if (v > m) m = v; return m; }
template <typename T>
T &chmin(T &m, const T &v) { if (v < m) m = v; return m; }

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
