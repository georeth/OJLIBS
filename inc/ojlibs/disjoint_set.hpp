#ifndef OJLIBS_INC_DISJOINT_SET_H_
#define OJLIBS_INC_DISJOINT_SET_H_

#include <vector>

struct disjoint_set {
    std::vector<int> parent;
    void reset(int size){
	parent.resize(size);
        for (int i = 0; i < parent.size(); i++){
            parent[i] = i;
        }
    }
    disjoint_set(int size){
        reset(size);
    }
    int find(int v){
        int pv = parent[v];
        return (pv == v) ? v : (parent[v] = find(pv));
    }
    bool union_if(int u, int v){
        int pu = find(u);
        int pv = find(v);
        if (pu == pv)
            return false;
        parent[pu] = pv;
        return true;
    }
};


#endif /* end of include guard: OJLIBS_INC_DISJOINT_SET_H_ */
