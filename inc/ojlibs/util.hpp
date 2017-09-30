#ifndef OJLIBS_INC_UTIL_H_
#define OJLIBS_INC_UTIL_H_

namespace ojlibs { // TO_BE_REMOVED

template <typename T>
bool chmax(T &m, const T &v) { if (v > m) return m = v, true; return false; }
template <typename T>
bool chmin(T &m, const T &v) { if (v < m) return m = v, true; return false; }
    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_UTIL_H_ */
