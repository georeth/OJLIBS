#ifndef OJLIBS_INC_SPARSE_LIL_H_
#define OJLIBS_INC_SPARSE_LIL_H_

#include <vector>
#include <string>
#include <ojlibs/util/iter_range.hpp>
#include <ojlibs/sparse/common.hpp>

namespace ojlibs { // TO_BE_REMOVED

// List of list (LIL) format, mapping edge to edge id

struct lil {
    int r, c;
    int esz = 0;
    std::vector<std::vector<sp_out_entry>> ijx;

    lil(int r = 0, int c = 0) : r(r), c(c), ijx(r) { }

    int esize() const { return esz; }
    int add(int i, int j) {
        ijx[i].push_back({j, esz});
        return esz++;
    }

    struct iter_base {
        iter_base(const std::vector<sp_out_entry> &jx, int idx) : jx(jx), idx(idx) { }
        iter_base &operator++() { ++idx; return *this; }
        bool operator==(const iter_base &that) const { return  idx == that.idx; }
        bool operator!=(const iter_base &that) const { return !(*this == that); }

        const std::vector<sp_out_entry> &jx;
        int idx;
    };
    struct iter : iter_base, sp_iter_base<sp_out_entry> {
        using iter_base::iter_base;
        value_type operator*() const {
            return jx[idx];
        }
    };
    struct vert_iter : iter_base, sp_iter_base<int> {
        using iter_base::iter_base;
        value_type operator*() const {
            return jx[idx].j;
        }
    };

    iter_range<iter> out(int i) const {
        auto &jx = ijx[i];
        return {{jx, 0}, {jx, (int)jx.size()}};
    }
    iter_range<vert_iter> outv(int i) const {
        auto &jx = ijx[i];
        return {{jx, 0}, {jx, (int)jx.size()}};
    }

};

std::string to_string(const lil &g) {
    std::string s = "lil " + std::to_string(g.r) + " " + std::to_string(g.c) + "\n";
    for (int row = 0; row < g.r; ++row) {
        s += std::to_string(row) + ": ";
        for (sp_out_entry e : g.ijx[row]) {
            s += std::to_string(e.j) + "(" + std::to_string(e.eid) + ")";
        }
        s += '\n';
    }
    return s;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_LIL_H_ */
