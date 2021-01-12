#ifndef OJLIBS_INC_SPARSE_SCC_H_
#define OJLIBS_INC_SPARSE_SCC_H_

#include <vector>

namespace ojlibs { // TO_BE_REMOVED

// (u, v) in E => scc_id[u] > scc_id[v]
struct scc_info {
    std::vector<int> scc_id;
    int nr_scc;
};

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_SPARSE_SCC_H_
