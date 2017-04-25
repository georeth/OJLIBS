#ifndef OJLIBS_INC_POWER_H_
#define OJLIBS_INC_POWER_H_

namespace ojlibs { // TO_BE_REMOVED

template <typename T, typename PInt>
T power(const T &x, PInt p, const T &id = 1) {
    T y = x;
    T r = id;
    while (p) {
        if (p & 1) r = r * y;
        y = y * y;
        p /= 2;
    }
    return r;
}

    
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_POWER_H_ */
