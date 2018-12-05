#ifndef OJLIBS_INC_COMPARABLE_H_
#define OJLIBS_INC_COMPARABLE_H_

namespace ojlibs { // TO_BE_REMOVED

// this is a crtp pattern
// struct T : comparable<T> { ... }
template <typename D>
struct comparable {
bool operator<(const comparable &that) const {
    return static_cast<const D &>(*this) < static_cast<const D &>(that);
}
bool operator==(const comparable &that) const {
    return (*this) < that && (that < *this);
}
bool operator!=(const comparable &that) const {
    return !(*this == that);
}
bool operator>(const comparable &that) const {
    return !(*this <= that);
}
bool operator<=(const comparable &that) const {
    return *this < that || *this == that;
}
bool operator>=(const comparable &that) const {
    return !(*this < that);
}
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_COMPARABLE_H_ */
