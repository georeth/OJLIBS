#ifndef OJLIBS_INC_SUM_INVERSION_H_
#define OJLIBS_INC_SUM_INVERSION_H_
// 1D : 
//      given f[*]
//      g[x] = sum{ y <= x } f[x]
// 2D :
//      given f[*][*]
//      g[x][y] = sum{ u <= x, v <= y } f[u][v]
//
//      g[x][y] = f[x][y] + g[x-1][y] + g[x][y-1] - g[x-1][y-1]
//      f[x][y] = g[x][y] - g[x-1][y] - g[x][y-1] + g[x-1][y-1]
//      (or two phase accumulation, easy to code)
//
//      where f[-1][*] = f[*][-1] = 0
//            g[-1][*] = g[*][-1] = 0
//
// change element in f : change region in g
//            +1  0  ==>  +1 +1 ...
//             0  0  F G  +1 +1 ...
//                        ...   ...
// change region in  g : change corners in f
//
//             0  0  ==>  +1 -1  0 ...
//             0 +1       -1 +1  0 ...
//                         0  0  0 ...
//
#include <vector>
#include <ojlibs/binary_operator.hpp>

namespace ojlibs { // TO_BE_REMOVED

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SUM_INVERSION_H_ */
