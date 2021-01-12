#ifndef OJLIBS_INC_UTIL_LOC_H_
#define OJLIBS_INC_UTIL_LOC_H_
#include <cassert>
#include <vector>

namespace ojlibs { // TO_BE_REMOVED

// v[idx]
template <typename T>
std::vector<T> loc(const std::vector<T> &v, const std::vector<int> &idx) {
    std::vector<T> ret(idx.size());
    for (size_t i = 0; i < idx.size(); ++i) {
        ret[i] = v[idx[i]];
    }
    return ret;
}

// v[idx^{-1}]
template <typename T>
std::vector<T> loc_rev(const std::vector<T> &v, const std::vector<int> &idx) {
    std::vector<T> ret(idx.size());
    for (size_t i = 0; i < idx.size(); ++i) {
        ret[idx[i]] = v[i];
    }
    return ret;
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_UTIL_LOC_H_
