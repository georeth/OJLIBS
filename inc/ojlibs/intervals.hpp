#ifndef OJLIBS_INC_INTERVALS_H_
#define OJLIBS_INC_INTERVALS_H_
// Given a set of intervals (ai, bi)
// let ci = (ai + bi) / 2
//     wi = (bi - ai) / 2
//
//      (aj, bj) < (ai, bi)  <=> bj <= ai
//      comparable <=> disjoint <=> |ci - cj| >= wi + wj
// For closed interval : map [x, y] to (2x, 2y + 1)

#include <vector>
#include <utility>
#include <algorithm>

namespace ojlibs { // TO_BE_REMOVED

// CLIQUE <=> LIS
// LIS has a O(n log n) algorithm: for each length, keep min x
// we can sort right boundary, so min x never decrease.
// O(n log n) sorting (no improvement...)
template <typename Int>
int interval_clique(std::vector<std::pair<Int, Int>> arr) {
    typedef std::pair<Int, Int> pair_t;
    if (arr.size() == 0) return 0;
    std::sort(begin(arr), end(arr),
            [](const pair_t &p1, const pair_t &p2) {
                return p1.second < p2.second;
            });

    int ans = 1;
    Int last = arr[0].second;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i].first >= last) {
            ++ans;
            last = arr[i].second;
        }
    }
    return ans;
}

// INDEPENDENT SET
// any independent set has a common intersection point
// any set has a common intersection point is independent
// O(n log n) sorting
template <typename Int>
int interval_independent_set(const std::vector<std::pair<Int, Int>> &arr) {
    std::vector<std::pair<int, bool>> v; // position, is start
    for (size_t i = 0; i < arr.size(); ++i) {
        v.push_back(make_pair(arr[i].first, true));
        v.push_back(make_pair(arr[i].second, false));
    }
    sort(begin(v), end(v));

    int ans = 0, cur = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].second) ++cur;
        else --cur;

        ans = std::max(ans, cur);
    }
    return ans;
}
    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_INTERVALS_H_ */
