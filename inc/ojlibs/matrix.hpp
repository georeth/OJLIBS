#ifndef OJLIBS_INC_MAT3_H_
#define OJLIBS_INC_MAT3_H_
#include <vector>
#include <cmath>
#include <cassert>
#include <ojlibs/arith.hpp>

// This library  supports common operations in linear algebra
// for different rings (groups).
// To make it easy to use, user is expected to use it through `linear'.
//
//   using L = linear<double>;
//   vector<double> v = {1.0 , 2.0, 3.0}
//   L::mat M = L::mat(3, 3);
//   auto M2 = M * M;
//
// Previously, we introduce `vector_view' to reference row or column of matrix.
//   M.row(2) = M.row(1) * 3; // not valid now
// But this feature has large overhead (allocation for temporary results) and
// N-element vector can be emulated by Nx1 matrix, so `vector_view' is deleted.
// Expression template is useful for this feature, but it is too heavy for this
// project.
// Now we access (i, j) element of matrix M by M(i, j).

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

#define TEMPL_ARG template <typename T, typename EleTraits>
#define TEMPL_ARG_DEFAULT \
    template <typename T, typename EleTraits = ring_traits<T>>
#define MAT_T matrix<T, EleTraits>

TEMPL_ARG_DEFAULT
struct matrix {
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
    T &operator()(int i, int j) { return arr[i * ncol + j]; }
    const T &operator()(int i, int j) const { return arr[i * ncol + j]; }
    static matrix eye(int n) {
        matrix mat(n, n);
        for (int i = 0; i < n; ++i)
            mat(i, i) = EleTraits::one();
        return mat;
    }
};

// A * B (no *=)
TEMPL_ARG
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
                        traits.time(left(i, k), right(k, j)));
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
    if (rr != right.r() || cc != right.c()) throw 0; \
 \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            traits.op_name##_assign(left(i, j), right(i, j)); \
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
    EleTraits traits;
    for (int i = 0; i < rr; ++i)
        for (int j = 0; j < cc; ++j)
            ret(i, j) = traits.negate(m(i, j));
    return ret;
}
#define MATRIX_SCALAR_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_ARG \
MAT_T &operator op_eq_str(MAT_T &left, const T &val) { \
    int rr = left.r(); \
    int cc = left.c(); \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) { \
        for (int j = 0; j < cc; ++j) { \
            traits.op_name##_assign(left(i, j), val); \
        } \
    } \
    return left; \
} \
TEMPL_ARG \
MAT_T operator op_str(const MAT_T &left, const T &val) { \
    MAT_T ret(left); \
    return ret op_eq_str val; \
} \
TEMPL_ARG \
MAT_T &operator op_eq_str(const T &val, MAT_T &right) { \
    int rr = right.r(); \
    int cc = right.c(); \
    EleTraits traits; \
    for (int i = 0; i < rr; ++i) \
        for (int j = 0; j < cc; ++j) \
            right(i, j) = traits.op_name(val, right(i, j)); \
    return right; \
} \
TEMPL_ARG \
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
TEMPL_ARG_DEFAULT
MAT_T permutation_matrix(const std::vector<int> &p) {
    int n = (int)p.size();
    EleTraits traits;
    MAT_T P(n, n);
    for (int i = 0; i < n; ++i)
        P(i, p[i]) = traits.one();
    return P;
}


// CORE

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
TEMPL_ARG
int LU_decompose(const MAT_T &A, MAT_T &L, MAT_T &U) {
    int n = A.r();
    EleTraits traits;
    if (n != A.c()) throw 0;

    L = MAT_T::eye(n);
    U = A;
    for (int j = 0; j < n; ++j) {
        if (U(j, j) == traits.zero())
            return -1;
        for (int i = j + 1; i < n; ++i) {
            T lij = traits.divide(U(i, j), U(j, j));
            L(i, j) = lij;
            // U[i] -= U[j] * lij
            U(i, j) = traits.zero();
            for (int k = j + 1; k < n; ++k)
                traits.minus_assign(U(i, k), U(j, k) * lij);
        }
    }
    return 0;
}

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
// PA = LU
// P is given by vector p
//         p[i] = j <=> P[i][j] = 1
TEMPL_ARG
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
        double max_norm = traits.norm(U(j, j));
        int max_index = j;

        // find largest pivot
        for (int jj = j + 1; jj < n; ++jj) {
            double cur_norm = traits.norm(U(jj, j));
            if (cur_norm > max_norm) {
                max_norm = cur_norm;
                max_index = jj;
            }
        }
        if (U(max_index, j) == traits.zero())
            return -1;
        if (j != max_index) {
            for (int k = 0; k < n; ++k) {
                std::swap(U(j, k), U(max_index, k));
                std::swap(L(j, k), L(max_index, k));
            }
            std::swap(p[j], p[max_index]);
            positive = !positive;
        }
        // as before
        for (int i = j + 1; i < n; ++i) {
            T lij = traits.divide(U(i, j), U(j, j));
            L(i, j) = lij;
            // U[i] -= U[j] * lij
            U(i, j) = 0;
            for (int k = j + 1; k < n; ++k)
                traits.minus_assign(U(i, k), traits.time(U(j, k), lij));
        }
    }
    for (int i = 0; i < n; ++i)
        L(i, i) = 1;
    return 0;
}

TEMPL_ARG
T determinant(const MAT_T &A) {
    assert(A.r() == A.c());

    EleTraits traits;
    std::vector<int> p;
    bool pos;
    MAT_T L, U;

    if (PLU_decompose(A, p, L, U, pos) < 0)
        return traits.zero();

    T ret = traits.one();
    for (int i = 0; i < A.r(); ++i)
        traits.time_assign(ret, U(i, i));
    if (!pos)
        ret = traits.negate(ret);
    return ret;
}

void print(const ojlibs::matrix<double> &m) {
    for (int i = 0; i < m.r(); ++i) {
	for (int j = 0; j < m.r(); ++j)
	    std::cout << m(i, j) << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// inverse my Guass elimination
TEMPL_ARG
MAT_T inverse(const MAT_T &A) {
    assert(A.r() == A.c());
    EleTraits traits;

    MAT_T U = A;
    int n = U.r();
    MAT_T R = MAT_T::eye(n);

    // first make U upper right
    for (int j = 0; j < n; ++j) {
        double max_norm = traits.norm(U(j, j));
        int max_index = j;

        // find largest pivot
        for (int jj = j + 1; jj < n; ++jj) {
            double cur_norm = traits.norm(U(jj, j));
            if (cur_norm > max_norm) {
                max_norm = cur_norm;
                max_index = jj;
            }
        }
        if (U(max_index, j) == traits.zero())
            throw 0;

        if (j != max_index) {
            for (int k = 0; k < n; ++k) {
                std::swap(U(j, k), U(max_index, k));
                std::swap(R(j, k), R(max_index, k));
            }
        }

        for (int i = j + 1; i < n; ++i) {
            T lij = traits.divide(U(i, j), U(j, j));
            U(i, j) = traits.zero();
            for (int k = j + 1; k < n; ++k)
                traits.minus_assign(U(i, k), traits.time(U(j, k), lij));
            for (int k = 0; k < n; ++k)
                traits.minus_assign(R(i, k), traits.time(R(j, k), lij));
        }
    }

    // then, make U diagonal
    for (int j = n - 1; j >= 0; --j) {
        for (int i = 0; i < j; ++i) {
            T lij = traits.divide(U(i, j), U(j, j));

            for (int k = 0; k < n; ++k)
                traits.minus_assign(R(i, k), traits.time(R(j, k), lij));
        }
    }

    // finally, make U identity
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            R(i, k) = traits.divide(R(i, k), U(i, i));

    return R;
}

#undef TEMPL_ARG
#undef TEMPL_ARG_DEFAULT
#undef MAT_T

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_MAT3_H_ */
