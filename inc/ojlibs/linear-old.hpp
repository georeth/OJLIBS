#ifndef OJLIBS_INC_LINEAR_H_
#define OJLIBS_INC_LINEAR_H_

#include <vector>

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

template <typename T, template <typename> class Ring = ring_traits>
struct matrix {
private:
    std::vector<std::vector<T>> data;
    static Ring<T> traits;
public:
    // construction
    matrix() = default;
    matrix(int r, int c)
        : data(r, std::vector<T>(c, traits.zero())) { }
    static matrix scalar(int r, const T &val) {
        matrix ret(r, r);
        for (int i = 0; i < r; ++i)
            ret.data[i][i] = val;
        return ret;
    }
    static matrix eye(int r) {
        return scalar(r, traits.one());
    }
    static matrix diagonal(const std::vector<T> &diag) {
        int r = (int)diag.size();
        matrix ret(r, r);
        for (int i = 0; i < r; ++i)
            ret.data[i][i] = diag[i];
        return ret;
    }
    void swap(matrix &that) { std::swap(data, that.data); }
    // accessor
    int r() const { return (int)data.size(); }
    int c() const {
        if (data.size())
            return (int)data[0].size();
        return 0;
    }
    const std::vector<T> &operator[](int m) const { return data[m]; }
    std::vector<T> &operator[](int m) { return data[m]; }
    bool operator== (const matrix &that) const {
        return data == that.data;
    }
    std::vector<T> column(int n) {
        std::vector<T> ret;
        ret.reserve(r());
        for (auto &row : data)
            ret.push_back(row[n]);
    }
    // modify
    void resize(int new_m, int new_n) {
        T zero = traits.zero();
        data.resize(new_m);
        for (auto &row : data)
            row.resize(new_n);
    }
    // operator (scalar)
    matrix &negate() {
        for (auto &row : data)
            for (T &ele : row)
                ele = traits.negate(ele);
        return *this;
    }
    matrix operator-() const {
        return matrix(*this).negate();
    }
#define OJLIBS_SCALAR_OP_ASSIGN(op_str, op_eq_str, op_name) \
    matrix &operator op_eq_str(const T &scalar) { \
        for (auto &row : data) \
            for (T &ele : row) \
                traits.op_name##_assign(ele, scalar); \
        return *this; \
    } \
    matrix &operator op_str(const T &scalar) const { \
        matrix ret(*this); \
        return ret op_eq_str scalar; \
    }
    OJLIBS_SCALAR_OP_ASSIGN(+, +=, plus)
    OJLIBS_SCALAR_OP_ASSIGN(-, -=, minus)
    OJLIBS_SCALAR_OP_ASSIGN(*, *=, time)
    OJLIBS_SCALAR_OP_ASSIGN(/, /=, divide)
#undef OJLIBS_SCALAR_OP_ASSIGN

    // operator (matrix)
#define OJLIBS_MATRIX_OP_ASSIGN(op_str, op_eq_str, op_name) \
    matrix &operator op_eq_str(const matrix &that) { \
        int rr = r(), cc = c(); \
        if (rr != that.r() || cc != that.c()) throw 0; \
        for (int i = 0; i < rr; ++i) { \
            for (int j = 0; j < cc; ++j) { \
                traits.op_name##_assign(data[i][j], that.data[i][j]); \
            } \
        } \
        return *this; \
    } \
    matrix &operator op_str(const matrix &that) const { \
        matrix ret(*this); \
        return ret op_eq_str that; \
    }
    OJLIBS_MATRIX_OP_ASSIGN(+, +=, plus)
    OJLIBS_MATRIX_OP_ASSIGN(-, -=, minus)
#undef OJLIBS_MATRIX_OP_ASSIGN
    matrix operator*(const matrix &that) const {
        int mm = r(), nn = c(), pp = that.c();
        const matrix &self = *this;
        if (nn != that.r()) throw 0;

        matrix ret(mm, pp);
        for (int i = 0; i < mm; ++i) {
            for (int j = 0; j < pp; ++j) {
                T acc = traits.zero();
                for (int k = 0; k < nn; ++k)
                    traits.plus_assign(acc,
                            traits.time(self.data[i][k], that.data[k][j]));
                ret.data[i][j] = acc;
            }
        }
        return ret;
    }
    matrix &operator *=(const matrix &that) {
        return *this = *this * that;
    }
};
template <typename T, template <typename> class Ring>
Ring<T> matrix<T, Ring>::traits;


// opeator non-member
template <typename T, template <typename> class Ring>
matrix<T, Ring> operator+(const T &value, const matrix<T, Ring> &p1) {
    matrix<T, Ring> ret(p1);
    return ret += value;
}
template <typename T, template <typename> class Ring>
matrix<T, Ring> operator*(const T &value, const matrix<T, Ring> &p1) {
    matrix<T, Ring> ret(p1);
    return ret *= value;
}
template <typename T, template <typename> class Ring>
matrix<T, Ring> operator-(const T &value, const matrix<T, Ring> &p1) {
    matrix<T, Ring> ret(p1);
    return ret.negate() += value;
}

template <typename T, template <typename> class Ring>
void swap(matrix<T, Ring> &p1, matrix<T, Ring> &p2) {
    p1.swap(p2);
}
// utility
template <typename T, template <typename> class Ring = ring_traits>
matrix<T, Ring> permutation_matrix(const std::vector<int> &p) {
    int n = (int)p.size();
    Ring<T> traits;
    matrix<T, Ring> P(n, n);
    for (int i = 0; i < n; ++i)
        P[i][p[i]] = traits.one();
    return P;
}

// core

// return 0 if succeed
// return -1 if pivots has zero (include the last one)
template <typename T, template <typename> class Ring>
int LU_decompose(const matrix<T, Ring> &A,
                 matrix<T, Ring> &L, matrix<T, Ring> &U) {
    int n = A.r();
    Ring<T> traits;
    if (n != A.c()) throw 0;

    L = matrix<T, Ring>::eye(n);
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
template <typename T, template <typename> class Ring>
int PLU_decompose(const matrix<T, Ring> &A, std::vector<int> &p,
                  matrix<T, Ring> &L, matrix<T, Ring> &U,
                  bool &positive) {
    int n = A.r();
    Ring<T> traits;
    if (n != A.c()) throw 0;

    positive = true;
    U = A;
    L = matrix<T, Ring>(n, n);
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

// LU_compact
// PLU_compact

// LDV
// PLDV

// LDV_compact
// PLDV_compact

template <typename T, template <typename> class Ring>
T determinant(const matrix<T, Ring> &A) {
    Ring<T> traits;
    std::vector<int> p;
    bool pos;
    matrix<T, Ring> L, U;

    if (PLU_decompose(A, p, L, U, pos) < 0)
        return traits.zero();

    T ret = traits.one();
    for (int i = 0; i < A.r(); ++i)
        traits.time_assign(ret, U[i][i]);
    if (!pos)
        ret = traits.negate(ret);
    return ret;
}

#endif /* end of include guard: OJLIBS_INC_LINEAR_H_ */
