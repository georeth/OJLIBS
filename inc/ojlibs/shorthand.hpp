/*
 * if you interested in the reference code here,
 * see https://github.com/georeth/OJLIBS for more information
 *
 * Solution Author : OJLIBS_CODER_NAME
 */

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
#include <ojlibs/int_range.hpp>

using namespace std;    // use as solution header. name polution is fine.
namespace ojlibs { namespace data_structure {}} // TO_BE_REMOVED
using namespace ojlibs;   // TO_BE_REMOVED
using namespace ojlibs::data_structure;   // TO_BE_REMOVED

#ifdef LOCAL_JUDGE
#  define pr_debug(...) fprintf(stderr, __VA_ARGS__)
#else
#  define pr_debug(...)
#endif

template <typename T>
T &chmax(T &m, const T &v) { if (v > m) m = v; return m; }
template <typename T>
T &chmin(T &m, const T &v) { if (v < m) m = v; return m; }

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
