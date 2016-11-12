/*
 * if you are interested in the reference code here,
 * see https://github.com/georeth/OJLIBS for more information
 *
 * Solution Author : OJLIBS_CODER_NAME
 */

#ifndef OJLIBS_INC_SHORTHAND_H_
#define OJLIBS_INC_SHORTHAND_H_
#include <cstdio>
#include <cmath>
#include <cstring>      // memset
#include <cstdlib>      // malloc
#include <cstdint>      // int64_t
#include <cinttypes>    // PRId64 SCNd64
#include <numeric>      // accumulate, partial_sum, inner_product, iota, adjacent_difference
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <tuple>        // pair, tuple
// #include <iostream>  // no iostream, many drawbacks (mix cstdio with fio, bad performance 2x)
#include <algorithm>
#include <ojlibs/int_range.hpp>

using namespace std;    // use as solution header. name polution is fine.
using namespace ojlibs;   // TO_BE_REMOVED

#ifdef LOCAL_JUDGE
#  define pr_debug(...) fprintf(stderr, __VA_ARGS__)
#else
#  define pr_debug(...)
#endif

template <typename T>
bool chmax(T &m, const T &v) { if (v > m) return m = v, true; return false; }
template <typename T>
bool chmin(T &m, const T &v) { if (v < m) return m = v, true; return false; }

/*
void fio() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}
*/

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
