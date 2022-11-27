#ifndef OJLIBS_INC_MICS_PREORDER_H_
#define OJLIBS_INC_MICS_PREORDER_H_

#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <ojlibs/ds/bitree.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <int D, typename ID = int>
struct preorder_solver_general {
    typedef std::array<int, D> coord_t;

    struct Q {
        coord_t coord;
        bool is_q;

        ID id;
        int64_t ans;
    };
    std::vector<Q> qs;

    // First, add query and update
    void add_query(coord_t coord, ID id) {
        qs.push_back(Q{coord, true, id, 0});
    }
    void add_update(coord_t coord) {
        qs.push_back(Q{coord, false, ID(), 0});
    }

    // Second, do discretize (optional)
    void discretize() {
        std::map<int, int> value[D];
        for (int d = 0; d < D; ++d) {
            for (const Q &q : qs) {
                value[d][q.coord[d]] = 0;
            }
        }
        for (int d = 0; d < D; ++d) {
            int i = 0;
            for (auto &kv : value[d])
                kv.second = i++;
        }
        for (int d = 0; d < D; ++d) {
            for (Q &q : qs) {
                q.coord[d] = value[d][q.coord[d]];
            }
        }
    }

    // Third, solve!
    typedef ojlibs::bitree<int64_t, ojlibs::binary_plus<int64_t>> bit_t;
    bit_t bit{0};

    void solve() {
        int max_last = 0;
        for (const Q &q : qs)
            if (q.coord[D - 1] > max_last) max_last = q.coord[D - 1];
        bit = bit_t(max_last + 1);

        std::vector<Q *> v(qs.size());
        for (size_t i = 0; i < qs.size(); ++i)
            v[i] = &qs[i];

        std::sort(v.begin(), v.end(), cmp(0));
        CDQ(0, v, 0, v.size());
    }
    void CDQ(int dep, std::vector<Q *> &v, size_t b, size_t e) {
        if (D - dep == 2) { // 2 dimensions left, use sort + fenwick
            solve_2D(v, b, e);
            return;
        }
        if (e - b <= 1) return;

        size_t m = (b + e) / 2;
        CDQ(dep, v, b, m);
        CDQ(dep, v, m, e);

        std::vector<Q *> vv;
        for (size_t i = b; i < m; ++i)
            if (!v[i]->is_q) vv.push_back(v[i]);
        for (size_t i = m; i < e; ++i)
            if (v[i]->is_q) vv.push_back(v[i]);

        std::sort(vv.begin(), vv.end(), cmp(dep + 1));
        CDQ(dep + 1, vv, 0, vv.size());
    }
    void solve_2D(std::vector<Q *> &v, size_t b, size_t e) {
        for (size_t i = b; i < e; ++i)
            if (!v[i]->is_q) {
                bit.increase_element(v[i]->coord[D - 1], 1);
            } else {
                v[i]->ans += bit.query_include(v[i]->coord[D - 1]);
            }

        for (size_t i = b; i < e; ++i)
            if (!v[i]->is_q)
                bit.increase_element(v[i]->coord[D - 1], -1);
    }

    void solve_2D_alter(std::vector<Q *> &v, size_t b, size_t e) {
        if (e - b <= 1) return;

        size_t m = (b + e) / 2;
        solve_2D_alter(v, b, m);
        solve_2D_alter(v, m, e);

        for (size_t i = b; i < m; ++i)
            if (!v[i]->is_q)
                bit.increase_element(v[i]->coord[D - 1], 1);
        for (size_t i = m; i < e; ++i)
            if (v[i]->is_q)
                v[i]->ans += bit.query_include(v[i]->coord[D - 1]);
        for (size_t i = b; i < m; ++i)
            if (!v[i]->is_q)
                bit.increase_element(v[i]->coord[D - 1], -1);
    }


    // helper functions
    struct cmp {
        int d;
        cmp(int d) : d(d) { }
        bool operator()(const Q *q1, const Q *q2) const {
            if (q1->coord[d] != q2->coord[d])
                return q1->coord[d] < q2->coord[d];
            return q1->is_q < q2->is_q;
        }
    };
};

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_MICS_PREORDER_H_
