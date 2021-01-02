#ifndef OJLIBS_INC_ALGEBRA_COMPLEX_H_
#define OJLIBS_INC_ALGEBRA_COMPLEX_H_

#include <cassert>
#include <complex>
#include <ojlibs/algebra/base.hpp>

namespace ojlibs { // TO_BE_REMOVED

struct complex_algebra : base_algebra<std::complex<double>> {
    using type = std::complex<double>;
    std::vector<type> unit_roots(int k, bool inv) const {
        std::vector<type> units;
        for (int m = 0; m <= k; ++m) {
            double alpha = 2 * M_PI / (1 << m);
            if (inv) alpha = -alpha;
            units.push_back({cos(alpha), sin(alpha)});
        }
        return units;
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_ALGEBRA_COMPLEX_H_ */
