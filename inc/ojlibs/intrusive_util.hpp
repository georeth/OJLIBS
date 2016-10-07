#ifndef OJLIBS_INC_INTRUSIVE_UTIL_H_
#define OJLIBS_INC_INTRUSIVE_UTIL_H_

namespace ojlibs { // TO_BE_REMOVED

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

struct key_identity {
    template<typename T>
    const T &operator() (const T &x) { return x; }
};
template <typename Head>
struct intrusive_traits {
    // head_type, iter_type
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_INTRUSIVE_UTIL_H_ */
