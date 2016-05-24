#ifndef OJLIBS_INC_UTIL_H_
#define OJLIBS_INC_UTIL_H_

#include <cstdint>
#include <utility>

namespace ojlibs {

// http://aggregate.org/MAGIC/
static inline unsigned all_ones(unsigned x) {
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
	return x;
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
static inline int msb32(unsigned int x) {
	x = all_ones(x);
        return x & ~(x >> 1);
}

struct key_identity {
	template<typename T>
	const T &operator() (const T &x) {
		return x;
	}
};

template<typename P, typename M>
uintptr_t member_ptr_offset(M P::*field) {
	P *parent = nullptr;
	M *member = &(parent->*field);
	return reinterpret_cast<uintptr_t>(member);
}

template<typename P, typename M>
P *member_to_parent(M *member, M P::*field) {
	uintptr_t ptr = reinterpret_cast<uintptr_t>(member) - member_ptr_offset(field);
	return reinterpret_cast<P *>(ptr);
}

} // ojlibs

#endif /* end of include guard: OJLIBS_INC_UTIL_H_ */
