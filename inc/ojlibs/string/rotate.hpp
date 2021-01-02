#ifndef OJLIBS_INC_STRING_ROTATE_H_
#define OJLIBS_INC_STRING_ROTATE_H_

#include <vector>
#include <algorithm>

namespace ojlibs { // TO_BE_REMOVED
// Paper: LEXICOGRAPHlCALLY LEAST CIRCULAR SUBSTRINGS                           TO_BE_REMOVED
// the algorithm is called Booth's algorithm.                                   TO_BE_REMOVED
// but i found the algorithm presented in the paper is wrong by random test:    TO_BE_REMOVED
//      aaabaaaaab -> baaaaabaaa, correct answer is baaabaaaaa                  TO_BE_REMOVED
// please check the wiki version (`failure` takes 2n space, no earlier quit):   TO_BE_REMOVED
//      https://en.wikipedia.org/wiki/Lexicographically_minimal_string_rotation TO_BE_REMOVED
//                                                                              TO_BE_REMOVED
// there is another algorithm that use O(1) storage, see optimal_rotation()     TO_BE_REMOVED

template <bool Max>
int booth_algorithm(const std::vector<int> &v_in) {
    size_t n = v_in.size();
    std::vector<int> v = v_in;
    v.insert(v.end(), v_in.begin(), v_in.end());
    std::vector<int> f(2 * n, -1);
    int k = 0;
    for (int j = 1; j < 2 * n; ++j) {
        int i = f[j - k - 1];
        while (i != -1 && v[j] != v[k + i + 1]) {
            assert(k + i + 1 < 2 * n);
            if ((v[j] < v[k + i + 1]) ^ Max) // #
                k = j - i - 1;
            i = f[i];
        }
        if (i == -1 && v[j] != v[k + i + 1]) {
            if ((v[j] < v[k + i + 1]) ^ Max) // #
                k = j;
            f[j - k] = -1;
        } else {
            f[j - k] = i + 1;
        }
    }
    return k;
}

template <bool Max>
int optimal_rotation(const std::vector<int> &v_in) {
    size_t n = v_in.size();
    std::vector<int> v = v_in;
    v.insert(v.end(), v_in.begin(), v_in.end());
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (v[i + k] == v[j + k])
            ++k;
        else if (Max ^ (v[i + k] < v[j + k]))
            j = j + k + 1, k = 0;
        else
            i = i + k + 1, k = 0;

        if (i == j)
            ++j;
    }
    assert(i < j);
    return std::min(i, j);
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_STRING_ROTATE_H_ */
