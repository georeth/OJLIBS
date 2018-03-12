#ifndef OJLIBS_INC_DISJOINT_SET_H_
#define OJLIBS_INC_DISJOINT_SET_H_

#include <vector>

namespace ojlibs { // TO_BE_REMOVED

struct disjoint_set {
    std::vector<int> parent;
    void reset(int size){
	parent.resize(size);
        for (int i = 0; i < parent.size(); i++){
            parent[i] = i;
        }
    }
    disjoint_set(int size){ reset(size); }
    int find(int c){
        int pc = parent[c];
        return (pc == c) ? c : (parent[c] = find(pc));
    }
    // ancestor(p) is parent of ancestor(c)
    bool union_if(int p, int c){
        int pp = find(p), pc = find(c);
        if (pp == pc) return false;
        parent[pc] = pp;
        return true;
    }
};

// extension: distance function and function f          TO_BE_REMOVED
//      dist(u, u) = 0                                  TO_BE_REMOVED
//      dist(u, w) = f(dist(u, v), dist(v, w))          TO_BE_REMOVED

} // ojlibs TO_BE_REMOVED


#endif /* end of include guard: OJLIBS_INC_DISJOINT_SET_H_ */
