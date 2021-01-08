#ifndef OJLIBS_INC_SPARSE_CSR_H_
#define OJLIBS_INC_SPARSE_CSR_H_

#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <ojlibs/iter_range.hpp>
#include <ojlibs/sparse/common.hpp>

namespace ojlibs { // TO_BE_REMOVED

// Compressed Sparse Row (CSR) format, and we allow duplicate edges.

template <typename...E>
void unzip_append(std::tuple<std::vector<E>...> &csr,
                const std::tuple<E...> &t) {
    std::apply([&](auto &... csr) {
        std::apply([&](const auto &... t) {
            (csr.push_back(t),...);
        }, t);
    }, csr);
}

template <typename...E>
std::tuple<std::vector<E>...>
unzip(std::vector<std::tuple<E...>> &vals) {
    std::tuple<std::vector<E>...> ret;
    for (const auto &t : vals)
        unzip_append(ret, t);
    return ret;
}

std::vector<int> counting_compress(const std::vector<int> &v, int n) {
    int idx = 0;
    std::vector<int> ix = {0}; // compressed row_idx
    for (int row = 0; row < n; ++row) {
        while (idx < v.size() && v[idx] == row)
            ++idx;
        ix.push_back(idx);
    }
    return ix;
}

struct csr {
    int r, c;
    std::vector<int> ix, jx;

    int esize() const { return (int)jx.size(); }

    struct iter_base {
        iter_base(const std::vector<int> &jx, int idx) : jx(jx), idx(idx) { }
        iter_base &operator++() { ++idx; return *this; }
        bool operator==(const iter_base &that) const { return  idx == that.idx; }
        bool operator!=(const iter_base &that) const { return !(*this == that); }

        const std::vector<int> &jx;
        int idx;
    };

    struct vert_iter : iter_base, sp_iter_base<int> {
        using iter_base::iter_base;
        value_type operator*() const {
            return jx[idx];
        }
    };
    struct iter : iter_base, sp_iter_base<sp_out_entry> {
        using iter_base::iter_base;
        value_type operator*() const {
            return {jx[idx], idx};
        }
    };

    iter_range<vert_iter> outv(int i) const {
        return {{jx, ix[i]}, {jx, ix[i + 1]}};
    }
    iter_range<iter> out(int i) const {
        return {{jx, ix[i]}, {jx, ix[i + 1]}};
    }
};

template <typename... E>
std::tuple<csr, std::vector<E>...>
to_csr(std::vector<std::tuple<int, int, E...>> coo, int r, int c) {
    std::sort(coo.begin(), coo.end(), [](const auto &t1, const auto &t2) {
            int c1 = std::get<0>(t1), c2 = std::get<0>(t2);
            if (c1 != c2) return c1 < c2;
            return std::get<1>(t1) < std::get<1>(t2);
    });

    std::tuple<csr, std::vector<E>...> ret;

    std::apply([&](auto &csr, auto &... vx) {
        tie(csr.ix, csr.jx, vx...) = unzip(coo); // copy-elision
        csr.r = r;
        csr.c = c;
        csr.ix = counting_compress(csr.ix, r);
    }, ret);

    return ret;
}

csr transpose(const csr &g) {
    csr ret;
    ret.r = g.c;
    ret.c = g.r;
    ret.jx.resize(g.jx.size()); // nnz

    std::vector<int> new_ix = g.jx;
    std::sort(new_ix.begin(), new_ix.end());
    new_ix = counting_compress(new_ix, ret.r);
    ret.ix = new_ix;

    int idx = 0;
    for (int row = 0; row < g.r; ++row) {
        for (; idx < g.ix[row + 1]; ++idx) {
            int col = g.jx[idx];
            ret.jx[new_ix[col]++] = row;
        }
    }
    return ret;
}

std::string to_string(const csr &g) {
    std::string s;
    int idx = 0;
    for (int row = 0; row < g.r; ++row) {
        s += std::to_string(row) + ": ";
        for (; idx < g.ix[row + 1]; ++idx) {
            int col = g.jx[idx];
            s += std::to_string(col) + ' ';
        }
        s += '\n';
    }
    return s;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_SPARSE_CSR_H_ */
