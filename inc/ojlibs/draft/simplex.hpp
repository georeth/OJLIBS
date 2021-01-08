#ifndef OJLIBS_INC_SIMPLEX_H_
#define OJLIBS_INC_SIMPLEX_H_

#include <cassert>
#include <vector>

namespace ojlibs { // TO_BE_REMOVED

// A * x <= b
// maximize g . x
bool simplex_lp(const std::vector<std::vector<double>> &A,
        const &std::vector<double> &b, 
        const &std::vector<double> &g,
        std::vector<double> &out_x) {
    int m = (int)A.size();      // #equation
    int n = (int)A[0].size();   // dimension
    assert(m > 0);
    assert(n > 0);
    assert(g.size() == n);
    assert(b.size() == m);

    std::vector<double> x(n);
    // TODO


    out_x = x;
    return true;
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SIMPLEX_H_ */
