#ifndef OJLIBS_INC_UTIL_ITER_RANGE_H_
#define OJLIBS_INC_UTIL_ITER_RANGE_H_

#include <utility>
#include <iterator>

namespace ojlibs { // TO_BE_REMOVED

template <typename It>
struct iter_range : std::pair<It, It> {
    typedef std::pair<It, It> base_t;
    typedef It iterator;
    using base_t::base_t;
    It begin() { return base_t::first; }
    It end() { return base_t::second; }
};

template <typename It>
iter_range<It> make_range(It b, It e) {
    return iter_range<It>(b, e);
}

template <typename It>
iter_range<std::reverse_iterator<It>> rev(const iter_range<It> &origin) {
    using rev_t = std::reverse_iterator<It>;
    return {rev_t(origin.second), rev_t(origin.first)};
}

// TODO: rev on std::string, etc.

} // ojlibs TO_BE_REMOVED

#endif//OJLIBS_INC_UTIL_ITER_RANGE_H_
