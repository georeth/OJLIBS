#ifndef OJLIBS_INC_STRING_KMP_H_
#define OJLIBS_INC_STRING_KMP_H_

#include <vector>
#include <iterator>

namespace ojlibs { // TO_BE_REMOVED

template <typename It>
std::vector<int> prefix_func(It b, It e) {
    // pre condition : b != e
    typename std::iterator_traits<It>::difference_type length = e - b;
    std::vector<int> prefix_func(length);
    // prefix_func[i] = { max j : j <= i, p[0,j[ = p]i-j, i] } (j is length)
    //                = { max j : prefix of p[0,i] = suffix p[1,i] length j } (p[0] exclueded)
    // prefix_func : max {suffix = prefix}
    prefix_func[0] = 0;

    // start from second character
    for (int i = 1; i < length; ++i) {
        int match = prefix_func[i - 1];
        while (match > 0 && b[i] != b[match])
            match = prefix_func[match - 1];
        if (b[i] == b[match]) ++match;
        prefix_func[i] = match;
    }
    return prefix_func;
}
template <typename It, typename PIt>
It kmp_match(It b, It e, PIt pattern, const std::vector<int> &prefix_func) {
    int match = 0;
    while (b != e) {
        while (match > 0 && *b != pattern[match])
            match = prefix_func[match - 1];
        if (*b == pattern[match]) ++match;
        if (match == prefix_func.size())
            return b - (prefix_func.size() - 1);
        ++b;
    }
    return b;
}

template <typename T, typename PIt>
int kmp_transit(int match, T chr, PIt pattern, const std::vector<int> &prefix_func) {
    if (match == prefix_func.size())
        match = prefix_func[match - 1];
    while (match > 0 && chr != pattern[match])
        match = prefix_func[match - 1];
    if (chr == pattern[match]) ++match;
    return match;
}

template <typename It, typename PIt>
std::vector<int> kmp_traverse(It b, It e, PIt pattern, const std::vector<int> &prefix_func) {
    typename std::iterator_traits<It>::difference_type length = e - b;
    std::vector<int> ret(length);
    auto output = ret.begin();
    // difference from build : start from the first character
    int match = 0;
    while (b != e) {
        if (match == prefix_func.size())
            match = prefix_func[match - 1];
        while (match > 0 && *b != pattern[match])
            match = prefix_func[match - 1];
        if (*b == pattern[match]) ++match;
        *(output++) = match;
        ++b;
    }
    return ret;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_STRING_KMP_H_ */
