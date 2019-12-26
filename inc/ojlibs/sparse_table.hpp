#ifndef OJLIBS_INC_SPARSE_TABLE_H_
#define OJLIBS_INC_SPARSE_TABLE_H_

#include <vector>
#include <algorithm>
#include <ojlibs/bit_trick.hpp>
#include <ojlibs/binary_operator.hpp>

namespace ojlibs { // TO_BE_REMOVED

template<typename T, typename Op = binary_max<T>>
struct sparse_table {
    // Op should be idempotent
    Op op;
    int size;
    std::vector<std::vector<T>> arr;

    explicit sparse_table(int size) : size(size), arr(floor_log2(size) + 1, std::vector<T>(size, op.identity())) { }
    std::vector<T> &data() {
        return arr[0];
    }
    T &operator[](int i) {
        return arr[0][i];
    }
    void rebuild() {
        for (int pi = 1; pi < arr.size(); ++pi)
            for (int i = 0; i + (1 << pi) <= size; ++i)
                arr[pi][i] = op(arr[pi - 1][i], arr[pi - 1][i + (1 << (pi - 1))]);
    }
    T query_range(int x1, int x2) {
        if (x1 >= x2) return op.identity();
        int px = floor_log2(x2 - x1);

        return op(arr[px][x1], arr[px][x2 - (1 << px)]);
    }
};


} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_TABLE_H_ */
