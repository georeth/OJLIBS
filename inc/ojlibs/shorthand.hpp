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
#include <ctime>        // clock
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
#include <numeric>      // iota, accumulate, partial_sum...

namespace ojlibs{}      // TO_BE_REMOVED
using namespace std;    // use as solution header. name polution is fine.
using namespace ojlibs;   // TO_BE_REMOVED

#ifdef LOCAL_JUDGE
#  define pr_debug(...) fprintf(stderr, __VA_ARGS__)
#  define pr_line() fprintf(stderr, "%s: %d\n", __FILE__, __LINE__)
#else
#  define pr_debug(...)
#  define pr_line()
#endif

#include <ojlibs/util.hpp>
#define ALL(x) begin(x), end(x)

#define FOR0(i,n) for(int i=0,i##end=n;i<i##end;i++)
#define FOR1(i,n) for(int i=1,i##end=n;i<=i##end;i++)
#define FOR(i,a,b) for(int i=a,i##end=b;i<=i##end;i++)
#define FOD0(i,n) for(int i=(n)-1;~i;i--)
#define FOD1(i,n) for(int i=n;i;i--)
#define FOD(i,a,b) for(int i=a,i##end=b;i>=i##end;i--)
#define FORE(i,x)for(__typeof(x.end())i=x.begin();i!=x.end();++i)

typedef int64_t ll;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<pair<int, int>> vii;
typedef vector<vector<int>> vvi;

// sadly, there's no portable way to get M_PI defined.
// it's not in standard (there's a C++ proposal).
// even _USE_MATH_DEFINES may fail when compiled with strict ANSI.
// I assume M_PI is a macro if it ever exists.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <ojlibs/fio.inc>

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
