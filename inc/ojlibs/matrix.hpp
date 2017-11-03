#ifndef OJLIBS_INC_MAT3_H_
#define OJLIBS_INC_MAT3_H_
#include <vector>
#include <cmath>
#include <cassert>
#include <ojlibs/arith.hpp>

namespace ojlibs { // TO_BE_REMOVED

// TYPES
template <typename T>
struct ring_traits {
    // using defualt operator==
    static T zero() { return T(); }
    static T negate(const T &p) { return -p; }
    static T plus(const T &p1, const T &p2) { return p1 + p2; }
    static T minus(const T &p1, const T &p2) { return p1 - p2; }
    static T one() { return 1; }
    static T time(const T &p1, const T &p2) { return p1 * p2; }

    static void plus_assign(T &p1, const T &p2) { p1 += p2; }
    static void minus_assign(T &p1, const T &p2) { p1 -= p2; }
    static void time_assign(T &p1, const T &p2) { p1 *= p2; }

    // optional
    static T inverse(const T &p) { return 1 / p; }
    static T divide(const T &p1, const T &p2) { return p1 / p2; }

    static void divide_assign(T &p1, const T &p2) { p1 /= p2; }

    // norm >= 0
    // norm == 0 <=> p == zero
    static double norm(const T &p) { return std::abs(p); }
};

template <typename Int = int, Int MOD = 1000000007>
struct modular_ring {
    // using defualt operator==
    static Int zero() { return 0; }
    static Int negate(const Int &p) { return subx_mod(0, p, MOD); }
    static Int plus(const Int &p1, const Int &p2) { return addx_mod(p1, p2, MOD); }
    static Int minus(const Int &p1, const Int &p2) { return subx_mod(p1, p2, MOD); }
    static Int one() { return 1; }
    static Int time(const Int &p1, const Int &p2) { return mul_mod(p1, p2, MOD); }

    static void plus_assign(Int &p1, const Int &p2) { p1 = plus(p1, p2); }
    static void minus_assign(Int &p1, const Int &p2) { p1 = minus(p1, p2); }
    static void time_assign(Int &p1, const Int &p2) { p1 = time(p1, p2); }

    // optional
    static Int inverse(const Int &p) { return inv_mod(p, MOD); }
    static Int divide(const Int &p1, const Int &p2) { return time(p1, inverse(p2)); }

    static void divide_assign(Int &p1, const Int &p2) { p1 = divide(p1, p2); }

    // norm >= 0
    // norm == 0 <=> p == zero
    static double norm(const Int &p) { return p; }
};

template <typename T,
          typename EleTraits = ring_traits<T>>
struct matrix {
    struct vector_view {
        T *data;
        int stride;
        T &operator[](int i) { return *(data + i * stride); }
        const T &operator[](int i) const { return *(data + i * stride); }
        vector_view(T *data, int stride) : data(data), stride(stride) { }
    };

    int nrow, ncol;
    std::vector<T> arr;
    matrix(int r = 0, int c = 0, const T &init = EleTraits::zero())
        : nrow(r), ncol(c), arr(nrow * ncol, init) { }
    bool operator==(const matrix &that) const {
        assert(nrow == that.nrow && ncol == that.ncol);
        for (int i = 0; i < nrow * ncol; ++i)
            if (arr[i] != that.arr[i]) return false;
        return true;
    }
    int r() const { return nrow; }
    int c() const { return ncol; }
    T *row(int i) { return arr.data() + i * ncol; }
    T const *row(int i) const { return arr.data() + i * ncol; }
    T *operator[](int i) { return row(i); }
    T const *operator[](int i) const { return row(i); }
    vector_view column(int i) { return vector_view(arr.data() + i, ncol); }
    vector_view const column(int i) const { return vector_view(static_cast<T *>(arr.data()) + i, ncol); }
    static matrix eye(int n) {
        matrix mat(n, n);
        for (int i = 0; i < n; ++i)
            mat[i][i] = EleTraits::one();
        return mat;
    }
};


#define TEMPL_MAT_ARG template <typename T, typename EleTraits>
#define TEMPL_MAT_ARG_DEFAULT \
    template <typename T, typename EleTraits = ring_traits<T>>
#define MAT_T matrix<T, EleTraits>

// A * B (no *=)
TEMPL_MAT_ARG
MAT_T operator*(const MAT_T &left, const MAT_T &right) {
    int mm = left.r(), nn = left.c(), pp = right.c();
    if (nn != right.r()) throw 0;

    EleTraits traits;

    MAT_T ret(mm, pp);
    for (int i = 0; i < mm; ++i) {
        for (int j = 0; j < pp; ++j) {
            T acc = traits.zero();
            for (int k = 0; k < nn; ++k)
                traits.plus_assign(acc,
                        traits.time(left[i][k], right[k][j]));
            ret[i][j] = acc;
        }
    }
    return ret;
}

// OPERATORS
#define MATRIX_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_MAT_ARG \
MAT_T &operator op_eq_str(MAT_T &left, const MAT_T &right) { \
    int rr = left.r(), cc = left.c(); \
    if (rr != right.r() || cc != right.c()) throw 0; \
 \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            traits.op_name##_assign(left[i][j], right[i][j]); \
    return left; \
} \
TEMPL_MAT_ARG \
MAT_T operator op_str(const MAT_T &left, const MAT_T &right) { \
    MAT_T ret(left); \
    return ret op_eq_str right; \
}
MATRIX_OP_GEN(+, +=, plus)
MATRIX_OP_GEN(-, -=, minus)
#undef MATRIX_OP_GEN

TEMPL_MAT_ARG
MAT_T operator-(const MAT_T &m) {
    int rr = m.r();
    int cc = m.c();

    MAT_T ret(m);
    EleTraits traits;
    for (int i = 0; i < rr; ++i)
        for (int j = 0; j < cc; ++j)
            ret[i][j] = traits.negate(m[i][j]);
    return ret;
}
#define MATRIX_SCALAR_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_MAT_ARG \
MAT_T &operator op_eq_str(MAT_T &left, const T &val) { \
    int rr = left.r(); \
    int cc = left.c(); \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) { \
        for (int j = 0; j < cc; ++j) { \
            traits.op_name##_assign(left[i][j], val); \
        } \
    } \
    return left; \
} \
TEMPL_MAT_ARG \
MAT_T operator op_str(const MAT_T &left, const T &val) { \
    MAT_T ret(left); \
    return ret op_eq_str val; \
} \
TEMPL_MAT_ARG \
MAT_T &operator op_eq_str(const T &val, MAT_T &right) { \
    int rr = right.r(); \
    int cc = right.c(); \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            right[i][j] = traits.op_name(val, right[i][j]); \
    return right; \
} \
TEMPL_MAT_ARG \
MAT_T operator op_str(const T &val, const MAT_T &right) { \
    MAT_T ret(right); \
    return val op_eq_str ret; \
}
MATRIX_SCALAR_OP_GEN(+, +=, plus)
MATRIX_SCALAR_OP_GEN(-, -=, minus)
MATRIX_SCALAR_OP_GEN(*, *=, time)
MATRIX_SCALAR_OP_GEN(/, /=, divide)
#undef MATRIX_SCALAR_OP_GEN

// HELPERS
TEMPL_MAT_ARG_DEFAULT
MAT_T permutation_matrix(const std::vector<int> &p) {
    int n = (int)p.size();
    EleTraits traits;
    MAT_T P(n, n);
    for (int i = 0; i < n; ++i)
        P[i][p[i]] = traits.one();
    return P;
}


// CORE

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
TEMPL_MAT_ARG
int LU_decompose(const MAT_T &A, MAT_T &L, MAT_T &U) {
    int n = A.r();
    EleTraits traits;
    if (n != A.c()) throw 0;

    L = MAT_T::eye(n);
    U = A;
    for (int j = 0; j < n; ++j) {
        if (U[j][j] == traits.zero())
            return -1;
        for (int i = j + 1; i < n; ++i) {
            T lij = traits.divide(U[i][j], U[j][j]);
            L[i][j] = lij;
            // U[i] -= U[j] * lij
            U[i][j] = 0;
            for (int k = j + 1; k < n; ++k)
                traits.minus_assign(U[i][k], U[j][k] * lij);
        }
    }
    return 0;
}

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
// PA = LU
// P is given by vector p
//         p[i] = j <=> P[i][j] = 1
TEMPL_MAT_ARG
int PLU_decompose(const MAT_T &A, std::vector<int> &p,
                  MAT_T &L, MAT_T &U, bool &positive) {
    int n = A.r();
    EleTraits traits;
    if (n != A.c()) throw 0;

    positive = true;
    U = A;
    L = MAT_T(n, n);
    p = std::vector<int>(n);
    for (int i = 0; i < n; ++i)
        p[i] = i;

    for (int j = 0; j < n; ++j) {
        double max_norm = traits.norm(U[j][j]);
        int max_index = j;

        // find largest pivot
        for (int jj = j + 1; jj < n; ++jj) {
            double cur_norm = traits.norm(U[jj][j]);
            if (cur_norm > max_norm) {
                max_norm = cur_norm;
                max_index = jj;
            }
        }
        if (U[max_index][j] == traits.zero())
            return -1;
        if (j != max_index) {
            for (int k = 0; k < n; ++k) {
                std::swap(U[j][k], U[max_index][k]);
                std::swap(L[j][k], L[max_index][k]);
            }
            std::swap(p[j], p[max_index]);
            positive = !positive;
        }
        // as before
        for (int i = j + 1; i < n; ++i) {
            T lij = traits.divide(U[i][j], U[j][j]);
            L[i][j] = lij;
            // U[i] -= U[j] * lij
            U[i][j] = 0;
            for (int k = j + 1; k < n; ++k)
                traits.minus_assign(U[i][k], U[j][k] * lij);
        }
    }
    for (int i = 0; i < n; ++i)
        L[i][i] = 1;
    return 0;
}

TEMPL_MAT_ARG
T determinant(const MAT_T &A) {
    EleTraits traits;
    std::vector<int> p;
    bool pos;
    MAT_T L, U;

    if (PLU_decompose(A, p, L, U, pos) < 0)
        return traits.zero();

    T ret = traits.one();
    for (int i = 0; i < A.r(); ++i)
        traits.time_assign(ret, U[i][i]);
    if (!pos)
        ret = traits.negate(ret);
    return ret;
}
#undef TEMPL_MAT_ARG
#undef TEMPL_MAT_ARG_DEFAULT
#undef MAT_T

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MAT3_H_ */
