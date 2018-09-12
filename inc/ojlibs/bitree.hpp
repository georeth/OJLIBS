#ifndef OJLIBS_INC_BITREE_H_
#define OJLIBS_INC_BITREE_H_

#include <vector>
#include <ojlibs/binary_operator.hpp>

namespace ojlibs { // TO_BE_REMOVED

// binary index tree / fenwick tree
template<typename T, typename Op = binary_plus<T>>
struct bitree {
    Op op;
    // vec[i] = sum of ]i-i&(-i),i]
    std::vector<T> vec;
    bitree(int size) : vec(size) { }
    T query_include(int e) {
        T ans = op.identity();
        if (e == -1) return ans;
        while (e != 0)
            ans = op(vec[e], ans), e -= e & -e;
        ans = op(vec[0], ans);
        return ans;
    }
    T query_range(int e) {
        return query_include(e - 1);
    }
    T query_element(int p) {
        return op(query_include(p), op.negate(query_range(p)));
    }
    void increase_element(int p, const T &n) {
        if (p == 0)
            vec[0] = op(vec[0], n);
        else {
            while (p < (int)vec.size()) {
                vec[p] = op(vec[p], n);
                p += p & -p;
            }
        }
    }
};
// inline int sum (int r) {
// 	int ans = 0;
// 	for (; r >= 0; r = (r & (r + 1)) - 1)
// 		ans += t[r];
// 		
// 	return ans;
// }
// inline int sum (int l, int r) {
//  	return sum(r) - sum(l - 1);
// }
// inline void inc (int i, int add) {
// 	for (; i < N; i = (i | (i + 1)))
// 		t[i] += add;
// }

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_BITREE_H_ */
