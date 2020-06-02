#ifndef OJLIBS_INC_INTRUSIVE_UTIL_H_
#define OJLIBS_INC_INTRUSIVE_UTIL_H_
#include <cstdint>
#include <functional>

namespace ojlibs { // TO_BE_REMOVED

template <typename P, typename M, M P::*field>
struct intrusive_helper {
    typedef P parent_type;
    typedef M member_type;
    static constexpr M P::*field_v = field;
    static uintptr_t get_offset() {
        P *parent = nullptr;
        M *member = &(parent->*field);
        return reinterpret_cast<uintptr_t>(member);
    }
    static M *to_member(P *p) { return &(p->*field); }
    static P *to_parent(M *m) {
        uintptr_t mm = reinterpret_cast<uintptr_t>(m);
        return reinterpret_cast<P *>(mm - get_offset());
    }
};

template <typename T>
struct identity_accessor {
    const T &operator() (const T &x) const { return x; }
};

template <typename P, typename M, M P::*field>
struct key_accessor {
    const M &operator() (const P &p) const { return p.*field; }
};

} // ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_INTRUSIVE_UTIL_H_ */
