#ifndef OJLIBS_INC_IDENTITY_VISITOR_H_
#define OJLIBS_INC_IDENTITY_VISITOR_H_
namespace ojlibs { // TO_BE_REMOVED

template <typename T>
struct identity_visitor { 
    // std::identity is not portable TO_BE_REMOVED
    static T &get(T &value) { return value; }
    static T get(const T &value) { return value; }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_IDENTITY_VISITOR_H_ */
