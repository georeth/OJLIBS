#ifndef OJLIBS_INC_Z_FUNC_H_
#define OJLIBS_INC_Z_FUNC_H_

#include <vector>
#include <iterator>
#include <algorithm>

namespace ojlibs { // TO_BE_REMOVED

// given a string s
// z_func[i] = { max len : s[0,len[ = s[i,i+len[ }
template <typename It>
std::vector<int> z_func(It b, It e) {
    typename std::iterator_traits<It>::difference_type length = e - b;
    std::vector<int> z(length);
    int l = 0, r = 0; // [l, r[
    for (int i = 1; i < length; ++i) {
        int ans = 0;
        if (i < r) // P[0,r-l[ = P[l,r[ => P[i-l,r-l[ = P[i,r[
            ans = std::min(r - i, z[i - l]);
        while (ans < length && b[ans] == b[i+ans]) ++ans;
        z[i] = ans;

        if (i + ans > r)
            l = i, r = i + ans;
    }
    return z;
}

} // namespace ojlibs TO_BE_REMOVED



#endif /* end of include guard: OJLIBS_INC_Z_FUNC_H_ */
