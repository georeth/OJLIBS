#ifndef OJLIBS_INC_MAT2_H_
#define OJLIBS_INC_MAT2_H_
#include <vector>
#include <cmath>

// TYPES
template <typename T>
struct ring_traits {
    // using defualt operator==
    static T zero() { return T(); }
    static T negate(const T &p) { return - p; }
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

template <typename T>
struct array_backend : private std::vector<std::vector<T>> {
    typedef std::vector<T> row_type;
    typedef std::vector<row_type> base_type;

    struct column_ref {
	base_type &arr;
	int j;
	T &operator[](int i) const { return arr[i][j]; }
    };

    array_backend() { }
    array_backend(int r, int c) : base_type(r, row_type(c)) { }
    array_backend(int r, int c, const T &v) : base_type(r, row_type(c, v)) { }
    bool operator ==(const array_backend &that) const {
	return static_cast<const base_type &>(*this)
	    == static_cast<const base_type &>(that);
    }
    bool operator !=(const array_backend &that) const {
	return !((*this) == that);
    }

    using base_type::operator[];
    int r() const { return (int)base_type::size(); }
    int c() const {
	if (!base_type::size()) return 0;
	return (int)(*this)[0].size();
    }
    row_type &row(int i) { return (*this)[i]; }
    const row_type &row(int i) const { return (*this)[i]; }
    column_ref column(int j) { return column_ref{*this, j}; }
    const column_ref column(int j) const { return column_ref{*this, j}; }
};
template <typename T, bool is_column = true>
struct vector_backend : private std::vector<T> {
    typedef std::vector<T> base_type;
    // base_type vec;
    /*
    vector_backend() { }
    vector_backend(int n) : vec(n) { }
    vector_backend(int n, const T &v) : vec(n, v) { }
    */
    using base_type::base_type;
    vector_backend() { }
    vector_backend(const base_type &data) : base_type(data) { }
    int r() const {
	if (is_column) return (int)this->size(); else return 1;
    }
    int c() const {
	if (is_column) return 1; else return (int)this->size();
    }
    T *row(int i) {
	return this->data() + (is_column ? i : 0);
    }
    const T *row(int i) const {
	return this->data() + (is_column ? i : 0);
    }
    T *column(int i) {
	return this->data() + (is_column ? 0 : i);
    }
    const T *column(int i) const {
	return this->data() + (is_column ? 0 : i);
    }
    T *operator[](int i) { return row(i); }
    const T *operator[](int i) const { return row(i); }
};
template <typename T>
using row_backend = vector_backend<T, false>;
template <typename T>
using column_backend = vector_backend<T, true>;

template <typename T,
	  template <typename> class EleTraits = ring_traits,
	  template <typename> class Backend = array_backend>
struct matrix : public Backend<T> {
    typedef Backend<T> base_type;
    using base_type::base_type;
    static matrix eye(int n) {
	EleTraits<T> traits;
	matrix m(n, n);
	for (int i = 0; i < n; ++i)
	    m[i][i] = traits.one();
	return m;
    }
};

#define TEMPL_MAT_ARG0 \
template <typename T, template <typename> class EleTraits>
#define TEMPL_MAT_ARG \
template <typename T, template <typename> class EleTraits, \
	  template <typename> class B>
#define TEMPL_MAT_ARG2 \
template <typename T, template <typename> class EleTraits, \
	  template <typename> class B1, \
	  template <typename> class B2>
#define MAT_T matrix<T, EleTraits, B>
#define MAT_T0 matrix<T, EleTraits, array_backend>
#define MAT_T1 matrix<T, EleTraits, B1>
#define MAT_T2 matrix<T, EleTraits, B2>
#define ROW_T row_vector<T, EleTraits>
#define COL_T column_vector<T, EleTraits>

template <typename T, 
	  template <typename> class EleTraits = ring_traits>
using column_vector = matrix<T, EleTraits, column_backend>;
template <typename T, 
	  template <typename> class EleTraits = ring_traits>
using row_vector = matrix<T, EleTraits, row_backend>;

// A * B
TEMPL_MAT_ARG2
MAT_T0 operator*(const MAT_T1 &left, const MAT_T2 &right) {
    int mm = left.r(), nn = left.c(), pp = right.c();
    if (nn != right.r()) throw 0;

    EleTraits<T> traits;
    MAT_T0 ret(mm, pp);
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

// A * x
TEMPL_MAT_ARG
COL_T operator*(const MAT_T &left, const COL_T &right) {
    int mm = left.r(), nn = left.c();
    if (nn != right.r()) throw 0;

    EleTraits<T> traits;
    column_vector<T, EleTraits> ret(mm);
    for (int i = 0; i < mm; ++i) {
	T acc = traits.zero();
	for (int k = 0; k < nn; ++k)
	    traits.plus_assign(acc,
		    traits.time(left[i][k], right[k][0]));
	ret[i][0] = acc;
    }
    return ret;
}
// yT * B
TEMPL_MAT_ARG
ROW_T operator*(const ROW_T &left, const MAT_T &right) {
    int nn = left.c(), pp = right.c();
    if (nn != right.r()) throw 0;

    EleTraits<T> traits;
    ROW_T ret(pp);
    for (int j = 0; j < pp; ++j) {
	T acc = traits.zero();
	for (int k = 0; k < nn; ++k)
	    traits.plus_assign(acc,
		    traits.time(left[0][k], right[k][j]));
	ret[0][j] = acc;
    }
    return ret;
}
// yT * x
TEMPL_MAT_ARG0
T operator*(const ROW_T &left, const COL_T &right) {
    int nn = left.c();
    if (nn != right.r()) throw 0;

    EleTraits<T> traits;
    T ret = traits.zero();
    for (int k = 0; k < nn; ++k)
	traits.plus_assign(ret,
		traits.time(left[0][k], right[k][0]));
    return ret;
}

// HELPERS
template <typename T,
	  template <typename> class EleTraits = ring_traits>
MAT_T0 permutation_matrix(const std::vector<int> &p) {
    int n = (int)p.size();
    EleTraits<T> traits;
    MAT_T0 P(n, n);
    for (int i = 0; i < n; ++i)
	P[i][p[i]] = traits.one();
    return P;
}

// OPERATORS

#define MATRIX_OP_GEN(op_str, op_eq_str, op_name) \
TEMPL_MAT_ARG2 \
MAT_T0 &operator op_eq_str(MAT_T1 &left, const MAT_T2 &right) { \
    int rr = left.r(), cc = left.c(); \
    if (rr != right.r() || cc != right.c()) throw 0; \
 \
    EleTraits<T> traits; \
    for (int i = 0; i < rr; ++i) \
	for (int j = 0; j < cc; ++j) \
	    traits.op_name##_assign(left[i][j], right[i][j]); \
    return left; \
} \
TEMPL_MAT_ARG2 \
MAT_T0 operator op_str(const MAT_T1 &left, const MAT_T2 &right) { \
    MAT_T0 ret(left); \
    return ret op_eq_str right; \
}
MATRIX_OP_GEN(+, +=, plus)
MATRIX_OP_GEN(-, -=, minus)
#undef MATRIX_OP_GEN

TEMPL_MAT_ARG
MAT_T0 operator-(const MAT_T &m) {
    int rr = m.r();
    int cc = m.c();

    MAT_T0 ret(m);
    EleTraits<T> traits;
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
    EleTraits<T> traits; \
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
    EleTraits<T> traits; \
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

// CORE

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
TEMPL_MAT_ARG
int LU_decompose(const MAT_T &A, MAT_T0 &L, MAT_T0 &U) {
    int n = A.r();
    EleTraits<T> traits;
    if (n != A.c()) throw 0;

    L = MAT_T0::eye(n);
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
// 	p[i] = j <=> P[i][j] = 1
TEMPL_MAT_ARG
int PLU_decompose(const MAT_T &A, std::vector<int> &p,
		  MAT_T0 &L, MAT_T0 &U, bool &positive) {
    int n = A.r();
    EleTraits<T> traits;
    if (n != A.c()) throw 0;

    positive = true;
    U = A;
    L = MAT_T0(n, n);
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
	    std::swap(U[j], U[max_index]);
	    std::swap(L[j], L[max_index]);
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
    EleTraits<T> traits;
    std::vector<int> p;
    bool pos;
    MAT_T0 L, U;

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
#undef TEMPL_MAT_ARG2
#undef MAT_T
#undef MAT_T0
#undef MAT_T1
#undef MAT_T2
#undef COL_T
#undef ROW_T

#endif /* end of include guard: OJLIBS_INC_MAT2_H_ */
