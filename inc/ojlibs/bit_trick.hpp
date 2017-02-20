#ifndef OJLIBS_INC_BIT_TRICK_H_
#define OJLIBS_INC_BIT_TRICK_H_

#include <cstdint>
#include <utility>

namespace ojlibs { // TO_BE_REMOVED

// http://aggregate.org/MAGIC/
static inline unsigned all_ones(unsigned x) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x;
}
static inline int floor_log2(unsigned x) {
    int ans = 0;
    while (x >>= 1)
	++ans;
    return ans;
}
static inline bool is_pow2(unsigned x) {
    return !(x & (x - 1));
}
static inline unsigned next_pow2(unsigned x) {
    return all_ones(x) + 1;
}
static inline unsigned ceil_pow2(unsigned x) {
    if (is_pow2(x)) return x;
    return next_pow2(x);
}
// floor pow2
static inline int msb32(unsigned int x) {
    x = all_ones(x);
    return x & ~(x >> 1);
}
static inline int lsb32(unsigned int x) {
    return x & -x;
}

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_BIT_TRICK_H_ */
