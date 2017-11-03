#ifndef OJLIBS_INC_ATTR_H_
#define OJLIBS_INC_ATTR_H_
namespace ojlibs { // TO_BE_REMOVED

template <typename T>
struct atom_attr {
    typedef T value_type;
    T w;
    atom_attr() { }
    atom_attr(const T &w) : w(w) { }
};
template <typename T>
struct atom_accessor {
    typedef atom_attr<T> AtomAttr;
    typedef typename AtomAttr::value_type value_type;
    value_type &operator()(AtomAttr &attr) const { return attr.w; }
    const value_type &operator()(const AtomAttr &attr) const { return attr.w; }
};

    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ATTR_H_ */
