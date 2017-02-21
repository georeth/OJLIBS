#ifndef OJLIBS_INC_ARITHMATIC_H_
#define OJLIBS_INC_ARITHMATIC_H_

#include <unordered_map>

namespace ojlibs { // TO_BE_REMOVED
namespace memorization { // TO_BE_REMOVED

// Policy : storage
template <typename MapType>
struct map_traits {
    typedef MapType map_type;
    typedef typename map_type::key_type key_type;
    typedef typename map_type::mapped_type mapped_type;
    static mapped_type map_get(const map_type &map, const key_type &key) {
        return map.at(key);
    }
    static bool map_exist(const map_type &map, const key_type &key) {
        return map.count(key);
    }
    static void map_set(map_type &map, const key_type &k, const mapped_type &v) {
        map[k] = v;
    }
};

template <typename Ret, typename Arg,
          template<typename K, typename V, typename ...> class Map = std::unordered_map>
class map_memo {
public:
    typedef Arg arg_type;
    typedef Ret ret_type;
    typedef Map<arg_type, ret_type> map_type;
    typedef map_traits<map_type> map_traits_type;

    virtual ~map_memo() { }
    ret_type operator() (const arg_type &arg) {
        if (traits.map_exist(map, arg))
            return traits.map_get(map, arg);
        ret_type ret = func(arg);
        traits.map_set(map, arg, ret);
        return ret;
    }
protected:
    virtual ret_type func(const arg_type &arg) = 0;
    map_traits_type traits;
    map_type map;
};

template <typename Derived, typename Ret, typename Arg,
	  template<typename K, typename V, typename ...> class Map = std::unordered_map>
class map_memo_crtp : public map_memo<Ret, Arg, Map> {
public:
    typedef Arg arg_type;
    typedef Ret ret_type;
    typedef Map<arg_type, ret_type> map_type;
    typedef map_traits<map_type> map_traits_type;
    typedef Derived derived_type;
    typedef map_memo<Ret, Arg, Map> base_type;

    // shadow : static dispatch
    ret_type operator() (const arg_type &arg) {
        if (this->traits.map_exist(this->map, arg))
            return this->traits.map_get(this->map, arg);
        ret_type ret = static_cast<Derived *>(this)->func_impl(arg);
        this->traits.map_set(this->map, arg, ret);
        return ret;
    }

    virtual ret_type func(const arg_type &arg) override {
        return static_cast<Derived *>(this)->func_impl(arg);
    }
private:
    Map<arg_type, ret_type> memory;
    // in Derived : ret_type func_impl(const arg_type &arg);
};

} // namespace memorization TO_BE_REMOVED
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_ARITHMATIC_H_ */
