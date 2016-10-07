#ifndef OJLIBS_INC_SPARSE_TABLE_H_
#define OJLIBS_INC_SPARSE_TABLE_H_

#include <vector>
#include <algorithm>
#include <ojlibs/bit_trick.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename T>
struct max_traits {
    T assoc(const T &left, const T &right) {
        return std::max(left, right);
    }
    void assoc_inplace(T &left, const T &right) {
        left = assoc(left, right);
    }
};

template<typename T, typename IdempotentTraits = max_traits<T> >
struct sparse_table {
    template <typename E>
    using tabular = std::vector<std::vector<E>>;
    IdempotentTraits traits;

    int r, c;
    tabular<tabular<T>> arr;
    sparse_table(int r, int c) : r(r), c(c) {
        tabular<T> table(r, std::vector<T>(c));
        arr.resize(floor_log2(r) + 1, std::vector<tabular<T>>(floor_log2(c) + 1, table));
    }
    std::vector<std::vector<T>> &data() {
        return arr[0][0];
    }
    void rebuild() {
        for (int pi = 0; pi < arr.size(); ++pi)
        for (int pj = 0; pj < arr[0].size(); ++pj)
            for (int i = 0; i + (1 << pi) <= r; ++i)
            for (int j = 0; j + (1 << pj) <= c; ++j) {
                if (pi == 0 && pj == 0) continue;
                else if (pi == 0) arr[pi][pj][i][j] = traits.assoc(arr[pi][pj - 1][i][j],
                        arr[pi][pj - 1][i][j + (1 << (pj - 1))]);
                else arr[pi][pj][i][j] = traits.assoc(arr[pi - 1][pj][i][j],
                        arr[pi - 1][pj][i + (1 << (pi - 1))][j]);
            }
    }
    T query_range(int x1, int y1, int x2, int y2) {
        if (x1 >= x2 || y1 >= y2) return T();
        int px = floor_log2(x2 - x1);
        int py = floor_log2(y2 - y1);

        const tabular<T> &tab = arr[px][py];
        T ans = tab[x1][y1];
        traits.assoc_inplace(ans, tab[x1][y2 - (1 << py)]);
        traits.assoc_inplace(ans, tab[x2 - (1 << px)][y1]);
        traits.assoc_inplace(ans, tab[x2 - (1 << px)][y2 - (1 << py)]);
        return ans;
    }
};


} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_TABLE_H_ */
