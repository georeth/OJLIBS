#ifndef OJLIBS_INC_PRUFER_H_
#define OJLIBS_INC_PRUFER_H_
#include <set>
#include <map>
#include <utility>
#include <vector>

namespace ojlibs { // TO_BE_REMOVED

static std::vector<int> tree_to_prufer(const std::vector<std::pair<int, int>> &tree) {
    // V : set of vertices
    // M : set of monovalent vertices
    // E : incident list
    // V = keys(E)
    std::set<int> M;
    std::map<int, std::set<int>> E;
    std::vector<int> prufer;

    for (const auto &e : tree) {
        E[e.first].insert(e.second);
        E[e.second].insert(e.first);
    }
    for (const auto &vs : E)
        if (vs.second.size() == 1)
            M.insert(vs.first);

    while (M.size() > 1) {
        int x = *M.begin(); // x is least monovalent vertex
        int y = *E[x].begin();
        prufer.push_back(y);

        M.erase(x);
        E[x].erase(y);
        E[y].erase(x);
        if (E[y].size() == 1)
            M.insert(y);
    }
    // size of prufer is (n - 1),
    // it's prufer coding *append last vertice index* !
    return prufer;
}

static std::vector<std::pair<int, int>> prufer_to_tree(const std::vector<int> &prufer) {
    // V : set of vertices [offset, offset + n[
    // D[u] : number of occurence (degree + 1) (0 is monovalent)
    // M : set of monovalent vertex
    int offset = prufer.back() - (int)prufer.size();
    std::vector<int> D(prufer.size() + 1);
    std::set<int> M;
    std::vector<std::pair<int, int>> tree;
    // init D
    for (int y : prufer)
        ++D[y - offset];
    // init M
    for (int i = 0; i < D.size(); ++i)
        if (!D[i]) M.insert(i + offset);
    for (int y : prufer) {
        int x = *M.begin(); // least monovalent vertex
        M.erase(x);
        if (!--D[y - offset]) M.insert(y);
        tree.push_back(std::make_pair(y, x));
    }
    return tree;
}

// if we know degree of each vertex,
// we can count the number of trees by pruder code.
// degree = occurence + 1
//
// let t(n; d1, ..., dn) is count of tree with n vertices, di is degree of i.
// t(n; d1, ..., dn) = C(n - 2; d1 - 1, d2 - 1, ..., dn - 1).
// so number of trees coincide with multinomial coefficients.
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_PRUFER_H_ */
