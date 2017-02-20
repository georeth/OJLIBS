#ifndef OJLIBS_INC_SUFFIX_ARRAY_H_
#define OJLIBS_INC_SUFFIX_ARRAY_H_

#include <vector>
#include <utility>
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
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SUFFIX_ARRAY_H_ */
