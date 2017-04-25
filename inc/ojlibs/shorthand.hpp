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
#include <climits>      // INT_MIN LLONG_MIN
#include <cfloat>       // DBL_MIN DBL_MAX
#include <cstring>      // memset
#include <cstdlib>      // malloc
#include <cstdint>      // int64_t
#include <cinttypes>    // PRId64 SCNd64
#include <cassert>      // assert
#include <limits>       // numeric_limits
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <tuple>        // pair, tuple
#include <iostream>     // be careful.
                        // mix with cstdio: slowwww...
                        // mix with cstdio + fastio: bug
#include <sstream>
#include <algorithm>
#include <ojlibs/int_range.hpp>

using namespace std;    // use as solution header. name polution is fine.
using namespace ojlibs;   // TO_BE_REMOVED

#ifdef LOCAL_JUDGE
#  define pr_debug(...) fprintf(stderr, __VA_ARGS__)
#  define pr_line() fprintf(stderr, "%s: %d\n", __FILE__, __LINE__)
#else
#  define pr_debug(...)
#  define pr_line()
#endif

template <typename T>
bool chmax(T &m, const T &v) { if (v > m) return m = v, true; return false; }
template <typename T>
bool chmin(T &m, const T &v) { if (v < m) return m = v, true; return false; }
template <typename T>
T max_s(const T &v) { return v; }
template <typename T, typename... Args>
T max_s(const T &v1, const T &v2, Args... arg) {
    return max_s(max(v1, v2), arg...);
}
template <typename T>
T min_s(const T &v) { return v; }
template <typename T, typename... Args>
T min_s(const T &v1, const T &v2, Args... arg) {
    return min_s(min(v1, v2), arg...);
}
static const int HMAX32 = INT32_MAX / 2;
static const int HMIN32 = -HMAX32;
static const int64_t HMAX64 = INT64_MAX / 2;
static const int64_t HMIN64 = -HMAX64;

#define fast_io\
    ios_base::sync_with_stdio(false); cin.tie(nullptr)
stringstream line_stream() {
    string str;
    getline(cin, str);
    return stringstream(str);
}
#define read_int(id) scanf("%d", &id)
#define read_int64(id) scanf("%" PRId64, &id)
#define new_int(id) int id; read_int(id)
#define new_int64(id) int64_t id; read_int64(id)
#define ALL(x) begin(x), end(x)

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
