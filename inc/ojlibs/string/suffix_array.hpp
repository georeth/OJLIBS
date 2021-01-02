#ifndef OJLIBS_INC_STRING_SUFFIX_ARRAY_H_
#define OJLIBS_INC_STRING_SUFFIX_ARRAY_H_

#include <vector>
#include <utility>
#include <algorithm>

namespace ojlibs { // TO_BE_REMOVED

// A simple O(N log N log N) algorithm using factor doubling and radix sort.
template <typename It>
std::vector<int> suffix_array(It b, It e) {
    int length = static_cast<int>(e - b); // random access iterator

    using order_pair = std::pair<int, int>;
    std::vector<std::pair<order_pair, int>> ans(length); // order_pair -> index
    std::vector<int> order(length); // index -> merged order

    for (int i = 0; i < length; ++i)
        ans[i] = std::make_pair(order_pair(b[i], 0), i);
    sort(ans.begin(), ans.end());

    for (int factor = 1; factor < length; factor *= 2) {
        // Renaming (combine the order)
        // <first order, second order> -> <combined order, 0>
        for (int i = 0, idx = 0; i < length; ++idx) {
            order_pair cur = ans[i].first;
            do
                ans[i++].first = std::make_pair(idx, 0);
            while (i < length && ans[i].first == cur);
        }
        // compute order array
        for (int i = 0; i < length; ++i)
            order[ans[i].second] = ans[i].first.first;

        // doubling the factor
        for (int i = 0; i < length; ++i) {
            int rest = ans[i].second + factor;
            // `$' is smallest (-INF, because signed char may be negative)
            ans[i].first.second = rest < length ? order[rest] : -1;
        }
        // MSD radix sort
        sort(ans.begin(), ans.end());
    }
    std::vector<int> ret(length);
    for (int i = 0; i < length; ++i)
        ret[i] = ans[i].second;
    return ret;
}
// Kasai's algorithm
// construct LCP from SA in O(n)
template <typename It>
std::vector<int> lcp_from_sa(It s, const std::vector<int> &sa) {
    int n = (int)sa.size();
    // sa  : rank     -> position
    // lcp : rank     -> lcp
    // rank: position -> rank
    std::vector<int> lcp(n);
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i)
        rank[sa[i]] = i;

    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (rank[i] == 0) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k])
            ++k;
        lcp[rank[i]] = k;
        if (k) --k;
    }
    return lcp;
}

// TODO: linear SA construction
//   DC3 or SA-IS

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_STRING_SUFFIX_ARRAY_H_ */
