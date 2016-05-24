#ifndef OJLIBS_INC_UTIL_H_
#define OJLIBS_INC_UTIL_H_

#include <cstdint>
#include <utility>

namespace ojlibs {

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
