#ifndef OJLIBS_INC_MAT3_H_
#define OJLIBS_INC_MAT3_H_
#include <vector>
#include <utility>
#include <cmath>
#include <cassert>
#include <ojlibs/algebra/base.hpp>

// This library  supports common operations in linear algebra
// for different rings (fields).

namespace ojlibs { // TO_BE_REMOVED

#define TEMPL_ARG template <typename Alg>
#define EleType typename Alg::type
#define MAT_T matrix<Alg>

TEMPL_ARG
struct matrix {
    int nrow, ncol;
    using T = typename Alg::type;
    std::vector<T> arr;
    Alg alg;

    explicit matrix(int r = 0, int c = 0, const Alg &alg_in = Alg())
        : nrow(r), ncol(c), arr(r * c, alg_in.zero()), alg(alg_in) { }
    bool operator==(const matrix &that) const {
        assert(nrow == that.nrow && ncol == that.ncol);
        for (int i = 0; i < nrow * ncol; ++i)
            if (arr[i] != that.arr[i]) return false;
        return true;
    }
    bool operator!=(const matrix &that) const {
        return !(*this == that);
    }
    int r() const { return nrow; }
    int c() const { return ncol; }
    T &operator()(int i, int j) { return arr[i * ncol + j]; }
    const T &operator()(int i, int j) const { return arr[i * ncol + j]; }
    static matrix eye(int n, const Alg &alg = Alg()) {
        matrix mat(n, n, alg);
        for (int i = 0; i < n; ++i)
            mat(i, i) = alg.one();
        return mat;
    }
};

// A * B (no *=)
TEMPL_ARG
MAT_T operator*(const MAT_T &left, const MAT_T &right) {
    int mm = left.r(), nn = left.c(), pp = right.c();
    assert(nn == right.r());

    auto &alg = left.alg;
    MAT_T ret(mm, pp, alg);
    for (int i = 0; i < mm; ++i) {
        for (int j = 0; j < pp; ++j) {
            auto acc = alg.zero();
            for (int k = 0; k < nn; ++k)
                acc = alg.plus(acc,
                        alg.multiplies(left(i, k), right(k, j)));
            ret(i, j) = acc;
        }
    }
    return ret;
}

// OPERATORS
#define MATRIX_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_ARG \
MAT_T &operator op_eq_str(MAT_T &left, const MAT_T &right) { \
    int rr = left.r(), cc = left.c(); \
    assert(rr == right.r() && cc == right.c()); \
 \
    auto &alg = left.alg; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            left(i, j) = alg.op_name(left(i, j), right(i, j)); \
    return left; \
} \
TEMPL_ARG \
MAT_T operator op_str(const MAT_T &left, const MAT_T &right) { \
    MAT_T ret(left); \
    return ret op_eq_str right; \
}
MATRIX_OP_GEN(+, +=, plus)
MATRIX_OP_GEN(-, -=, minus)
#undef MATRIX_OP_GEN

TEMPL_ARG
MAT_T operator-(const MAT_T &m) {
    int rr = m.r();
    int cc = m.c();

    MAT_T ret(m);
    auto &alg = m.alg;
    for (int i = 0; i < rr; ++i)
        for (int j = 0; j < cc; ++j)
            ret(i, j) = alg.negate(m(i, j));
    return ret;
}
#define MATRIX_SCALAR_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_ARG \
MAT_T &operator op_eq_str(MAT_T &left, const EleType &val) { \
    int rr = left.r(); \
    int cc = left.c(); \
    auto &alg = left.alg; \
    for (int i = 0; i < rr; ++i) { \
        for (int j = 0; j < cc; ++j) { \
            left(i, j) = alg.op_name(left(i, j), val); \
        } \
    } \
    return left; \
} \
TEMPL_ARG \
MAT_T operator op_str(const MAT_T &left, const EleType &val) { \
    MAT_T ret(left); \
    return ret op_eq_str val; \
} \
TEMPL_ARG \
MAT_T &operator op_eq_str(const EleType &val, MAT_T &right) { \
    int rr = right.r(); \
    int cc = right.c(); \
    auto &alg = right.alg; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            right(i, j) = alg.op_name(val, right(i, j)); \
    return right; \
} \
TEMPL_ARG \
MAT_T operator op_str(const EleType &val, const MAT_T &right) { \
    MAT_T ret(right); \
    return val op_eq_str ret; \
}
MATRIX_SCALAR_OP_GEN(+, +=, plus)
MATRIX_SCALAR_OP_GEN(-, -=, minus)
MATRIX_SCALAR_OP_GEN(*, *=, multiplies)
MATRIX_SCALAR_OP_GEN(/, /=, divides)
#undef MATRIX_SCALAR_OP_GEN

// HELPERS
TEMPL_ARG
MAT_T permutation_matrix(const std::vector<int> &p, const Alg &alg = Alg()) {
    int n = (int)p.size();
    MAT_T P(n, n, alg);
    for (int i = 0; i < n; ++i)
        P(i, p[i]) = alg.one();
    return P;
}


// CORE

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
TEMPL_ARG
int LU_decompose(const MAT_T &A, MAT_T &L, MAT_T &U) {
    int n = A.r();
    auto &alg = A.alg;
    assert(n == A.c());

    L = MAT_T::eye(n, alg);
    U = A;
    for (int j = 0; j < n; ++j) {
        if (U(j, j) == alg.zero())
            return -1;
        for (int i = j + 1; i < n; ++i) {
            auto lij = alg.divides(U(i, j), U(j, j));
            L(i, j) = lij;
            // U[i] -= U[j] * lij
            U(i, j) = alg.zero();
            for (int k = j + 1; k < n; ++k)
                U(i, k) = alg.minus(U(i, k), alg.multiplies(U(j, k), lij));
        }
    }
    return 0;
}

// PA = LU
// A[m x n]: input matrix to be decomposed
// P is permutation matrix given by vector p: p[i] = j <=> P[i][j] = 1
// L[m x r]: lower triangular, last (m - rank) columns only has 1 on diagonal
// U[r x n]: upper triangular in row echelon form, last (n - r) rows are zero
// pivot_index: pivots located at U(p, pivot_index[p]). rank == pivot_index.size()
TEMPL_ARG
struct PLU_info {
    std::vector<int> p;
    MAT_T L, U;
    std::vector<int> pivot_index;
};

TEMPL_ARG
PLU_info<Alg> PLU_decompose(const MAT_T &A) {
    int r = A.r(), c = A.c();
    auto &alg = A.alg;

    MAT_T U = A;
    MAT_T L = MAT_T(r, r, alg);
    std::vector<int> p(r);
    std::vector<int> pivot_index(r);
    for (int i = 0; i < r; ++i)
        p[i] = i;

    int i = 0;
    for (int j = 0; i < r && j < c; ++j) {
        auto max_norm = alg.norm(U(i, j));
        int max_index = i;

        // find largest pivot
        for (int ii = i + 1; ii < r; ++ii) {
            auto cur_norm = alg.norm(U(ii, j));
            if (cur_norm > max_norm) {
                max_norm = cur_norm;
                max_index = ii;
            }
        }
        if (U(max_index, j) == alg.zero())
            continue;

        pivot_index[i] = j;
        if (i != max_index) {
            for (int k = 0; k < j; ++k)
                std::swap(L(i, k), L(max_index, k));
            for (int k = j; k < c; ++k)
                std::swap(U(i, k), U(max_index, k));
            std::swap(p[i], p[max_index]);
        }
        // as before
        for (int ii = i + 1; ii < r; ++ii) {
            auto li = alg.divides(U(ii, j), U(i, j));
            // fprintf(stderr, "set lij %d %d = %d\n", ii, j, lij);
            L(ii, i) = li;
            // U[i] -= U[j] * lij
            U(ii, j) = 0;
            for (int k = j + 1; k < c; ++k)
                U(ii, k) = alg.minus(U(ii, k), alg.multiplies(U(i, k), li));
        }
        ++i;
    }
    pivot_index.resize(i);
    for (int ii = 0; ii < r; ++ii)
        L(ii, ii) = 1;

    return PLU_info<Alg>{p, L, U, pivot_index};
}

TEMPL_ARG
MAT_T PLU_solve_general(const PLU_info<Alg> &plu) {
    auto &U = plu.U;
    auto &pivots = plu.pivot_index;
    auto &alg = U.alg;
    MAT_T ret(U.c(), U.c() - (int)pivots.size(), alg);

    int next_pivot = 0;
    for (int j = 0; j < U.c(); ++j) {
        if (next_pivot < pivots.size() && pivots[next_pivot] == j)
            ++next_pivot;
        else {
            ret(j, j - next_pivot) = alg.one();
            for (int p = next_pivot - 1; p >= 0; --p) {
                auto t = U(p, j);
                for (int pp = p; pp < next_pivot; ++pp)
                    t = alg.plus(t, alg.multiplies(ret(pivots[pp], j - next_pivot), U(p, pivots[pp])));
                ret(pivots[p], j - next_pivot) = alg.negate(alg.divides(t, U(p, pivots[p])));
            }
        }
    }
    return ret;
}

TEMPL_ARG
std::pair<MAT_T, bool> PLU_solve_special(const PLU_info<Alg> &plu, const MAT_T &y) {
    assert(y.c() == 1);

    auto &&[p, L, U, pivots] = plu;
    auto &alg = U.alg;
    int m = U.r();
    int n = U.c();
    int rank = (int)plu.pivot_index.size();

    MAT_T x(n, 1, alg);

    // solve Lz = Py
    MAT_T z(m, 1, alg);
    for (int i = 0; i < m; ++i) {
        auto t = y(p[i], 0);
        // auto t = py(i, 0);
        for (int j = 0; j < i; ++j)
            t = alg.minus(t, alg.multiplies(L(i, j), z(j, 0)));
        z(i, 0) = t;

        if (i >= rank && t != alg.zero())
            return {x, false};
    }
    // assert(L * z == py);

    // solve Ux = z
    for (int i = rank - 1; i >= 0; --i) {
        auto t = z(i, 0);
        for (int j = i + 1; j < rank; ++j)
            t = alg.minus(t, alg.multiplies(U(i, pivots[j]), x(pivots[j], 0)));

        int p = pivots[i];
        x(p, 0) = alg.divides(t, U(i, p));
    }
    // assert(U * x == z);

    return {x, true};
}


TEMPL_ARG
EleType determinant(const MAT_T &A) {
    assert(A.r() == A.c());
    auto &alg = A.alg;

    auto info = PLU_decompose(A);
    auto &p = info.p;
    auto &U = info.U;
    if (info.pivot_index.size() < A.r())
        return alg.zero();

    bool pos = true;
    for (int i = 0; i < A.r(); ++i)
        for (int j = 0; j < i; ++j)
            if (p[j] > p[i])
                pos = !pos;


    auto ret = alg.one();
    for (int i = 0; i < A.r(); ++i)
        ret = alg.multiplies(ret, U(i, i));
    if (!pos)
        ret = alg.negate(ret);
    return ret;
}

// inverse by Guass elimination
TEMPL_ARG
MAT_T inverse(const MAT_T &A) {
    assert(A.r() == A.c());
    auto &alg = A.alg;

    MAT_T U = A;
    int n = U.r();
    MAT_T R = MAT_T::eye(n, alg);

    // first make U upper right
    for (int j = 0; j < n; ++j) {
        auto max_norm = alg.norm(U(j, j));
        int max_index = j;

        // find largest pivot
        for (int jj = j + 1; jj < n; ++jj) {
            auto cur_norm = alg.norm(U(jj, j));
            if (cur_norm > max_norm) {
                max_norm = cur_norm;
                max_index = jj;
            }
        }
        assert(U(max_index, j) != alg.zero());

        if (j != max_index) {
            for (int k = 0; k < n; ++k) {
                std::swap(U(j, k), U(max_index, k));
                std::swap(R(j, k), R(max_index, k));
            }
        }

        for (int i = j + 1; i < n; ++i) {
            auto lij = alg.divides(U(i, j), U(j, j));
            U(i, j) = alg.zero();
            for (int k = j + 1; k < n; ++k)
                U(i, k) = alg.minus(U(i, k), alg.multiplies(U(j, k), lij));
            for (int k = 0; k < n; ++k)
                R(i, k) = alg.minus(R(i, k), alg.multiplies(R(j, k), lij));
        }
    }

    // then, make U diagonal
    for (int j = n - 1; j >= 0; --j) {
        for (int i = 0; i < j; ++i) {
            auto lij = alg.divides(U(i, j), U(j, j));

            for (int k = 0; k < n; ++k)
                R(i, k) = alg.minus(R(i, k), alg.multiplies(R(j, k), lij));
        }
    }

    // finally, make U identity
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            R(i, k) = alg.divides(R(i, k), U(i, i));

    return R;
}

#undef TEMPL_ARG
#undef EleType
#undef MAT_T

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MAT3_H_ */
