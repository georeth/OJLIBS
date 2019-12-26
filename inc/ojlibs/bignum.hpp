// A C++ big integer implementation derived from golang math/big

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
#ifndef OJLIBS_INC_BIGNUM_H_
#define OJLIBS_INC_BIGNUM_H_

#include <cinttypes>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <ojlibs/power.hpp>
#include <ojlibs/bit_trick.hpp>

namespace ojlibs { // TO_BE_REMOVED
namespace bignum {
#include <ojlibs/bignum_arith.inc>

static const size_t KARA_THRES = 40;

void clear(SPAN z) {
    for (size_t i = 0; i < z.n; ++i)
        z.p[i] = 0;
}
CSPAN norm(CSPAN z) {
    while (z.n > 0 && z.p[z.n - 1] == 0)
        --z.n;
    return {z.p, z.n};
}
SPAN norm(SPAN z) {
    CSPAN cz = norm(z.to_cspan());
    return {z.p, cz.n};
}
int cmp(CSPAN x, CSPAN y) {
    if (x.n != y.n) return (x.n < y.n) ? -1 : 1;

    for (size_t i = x.n; i-- > 0;)
        if (x.p[i] != y.p[i]) return (x.p[i] < y.p[i]) ? -1 : 1;
    return 0;
}
void addAt(SPAN z, CSPAN x, size_t i) {
    size_t n = x.n;
    if (n > 0) {
        WORD c;
        addVV({z.p + i, n}, z.p + i, x.p, &c);
        if (c) {
            size_t j = i + n;
            if (j < z.n)
                addVW({z.p + j, z.n - j}, z.p + j, c, &c);
        }
    }
}
void basicMul(SPAN z, CSPAN x, CSPAN y) {
    clear(z);
    for (size_t i = 0; i < y.n; ++i) {
        WORD d = y.p[i];
        if (d)
            addMulVVW({z.p + i, x.n}, x.p, d, &z.p[x.n + i]);
    }
}

void karatsubaAdd(SPAN z, CSPAN x, size_t n) {
    WORD c;
    addVV({z.p, n}, z.p, x.p, &c);
    if (c)
        addVW({z.p + n, n >> 1}, z.p + n, c, &c);
}

void karatsubaSub(SPAN z, CSPAN x, size_t n) {
    WORD c;
    subVV({z.p, n}, z.p, x.p, &c);
    if (c)
        subVW({z.p + n, n >> 1}, z.p + n, c, &c);
}

void karatsuba(SPAN z, CSPAN x, CSPAN y) {
    // all limb sizes depend on y
    size_t n = y.n;

    if ((n & 1) || n < KARA_THRES || n < 2) {
        basicMul(z, x, y);
        return;
    }

    size_t n2 = n >> 1;
    CSPAN x1 = {x.p + n2, n2}, x0 = {x.p, n2};
    CSPAN y1 = {y.p + n2, n2}, y0 = {y.p, n2};
    SPAN z2 = {z.p + n, z.n - n};

    karatsuba(z, x0, y0);
    karatsuba(z2, x1, y1);

    int s = 1;
    SPAN xd = {z.p + 2 * n, n2};

    WORD c;
    subVV(xd, x1.p, x0.p, &c);
    if (c) {
        s = -s;
        subVV(xd, x0.p, x1.p, &c);
    }

    SPAN yd = {z.p + 2 * n + n2, n2};
    subVV(yd, y0.p, y1.p, &c);
    if (c) {
        s = -s;
        subVV(yd, y1.p, y0.p, &c);
    }

    SPAN p = {z.p + 3 * n, z.n - 3 * n};
    karatsuba(p, xd.to_cspan(), yd.to_cspan());

    SPAN r{z.p + n * 4, z.n - n * 4};
    std::copy(z.p, z.p + 2 * n, r.p);

    karatsubaAdd({z.p + n2, z.n - n2}, r.to_cspan(), n);
    karatsubaAdd({z.p + n2, z.n - n2}, {r.p + n, r.n - n}, n);

    if (s > 0)
        karatsubaAdd({z.p + n2, z.n - n2}, p.to_cspan(), n);
    else
        karatsubaSub({z.p + n2, z.n - n2}, p.to_cspan(), n);
}

size_t karatsubaLen(size_t n, size_t thres) {
    size_t len = 0;
    while (n > thres) {
        n >>= 1;
        ++len;
    }
    return n << len;
}

struct nat {
    static const int RESERVE_SIZE = 4;
    VEC limbs;

    SPAN to_span(size_t b = 0, size_t sz = SIZE_MAX) {
        return SPAN{limbs.data() + b, std::min(limbs.size() - b, sz)};
    }

    CSPAN to_cspan(size_t b = 0, size_t sz = SIZE_MAX) const {
        return CSPAN{limbs.data() + b, std::min(limbs.size() - b, sz)};
    }

    void norm() {
        limbs.resize(bignum::norm(to_span()).n);
    }

    size_t nlimb() const { return limbs.size(); }

    nat &operator =(WORD word) { 
        if (word != 0)
            limbs = {word};
        return *this;
    }
    nat &operator =(DWORD dword) {
        WORD hi = WORD(dword >> WBITS), lo = WORD(dword);
        if (hi) limbs = {lo, hi};
        else if (lo) limbs = {lo};
        return *this;
    }
    explicit nat() { }
    explicit nat(WORD word) { *this = word; }
    explicit nat(DWORD dword) { *this = dword; }
    explicit nat(const VEC &limbs) : limbs(limbs) { }
    explicit nat(CSPAN sp) : limbs(sp.p, sp.p + sp.n) { }
    void make_size(size_t sz, size_t reserve = RESERVE_SIZE) {
        limbs.resize(sz);
        limbs.reserve(sz + reserve);
    }

    static nat add_inner(CSPAN x, CSPAN y) {
        nat sum;
        sum.limbs.resize(x.n + 1);

        WORD c;
        addVV(sum.to_span(0, y.n), x.p, y.p, &c);
        if (x.n > y.n)
            addVW(sum.to_span(y.n, x.n - y.n), x.p + y.n, c, &c);
        sum.limbs[x.n] = c;
        sum.norm();
        return sum;
    }

    nat operator+(const nat &that) const {
        if (nlimb() < that.nlimb())
            return add_inner(that.to_cspan(), to_cspan());
        else
            return add_inner(to_cspan(), that.to_cspan());
    }

    static nat sub(CSPAN x, CSPAN y) {
        size_t m = x.n, n = y.n;
        if (n > m) throw 0;
        if (m == 0) return nat();
        if (n == 0) return nat(x);

        nat z; z.limbs.resize(m);
        WORD c;
        subVV(z.to_span(0, n), x.p, y.p, &c);
        if (m > n)
            subVW(z.to_span(n), x.p + n, c, &c);
        if (c) throw 0;

        z.norm();
        return z;
    }

    nat operator-(const nat &that) const {
        return sub(to_cspan(), that.to_cspan());
    }
#define CMP_OP(op) bool operator op (const nat &that) const { return cmp(that) op 0; }
    CMP_OP(<) CMP_OP(<=) CMP_OP(>) CMP_OP(>=) CMP_OP(==) CMP_OP(!=)
#undef CMP_OP

    int cmp(const nat &that) const {
        return bignum::cmp(to_cspan(), that.to_cspan());
    }

    static nat mulAddWW(CSPAN x, WORD y, WORD r) {
        size_t m = x.n;
        if (m == 0 || y == 0)
            return nat(r);

        nat z;
        z.limbs.resize(m + 1);
        mulAddVWW(z.to_span(0, m), x.p, y, r, &z.limbs[m]);
        z.norm();

        return z;
    }

    static nat mul(CSPAN x, CSPAN y) {
        size_t m = x.n, n = y.n;
        if (m < n) return mul(y, x);
        else if (m == 0 || n == 0) return nat();
        if (n == 1) return mulAddWW(x, y.p[0], 0);

        if (n < KARA_THRES) {
            nat z;
            z.limbs.resize(m + n);
            basicMul(z.to_span(), x, y);
            z.norm();
            return z;
        }

        size_t k = karatsubaLen(n, KARA_THRES);
        CSPAN x0 = {x.p, k},
              y0 = {y.p, k};

        nat z;
        z.limbs.resize(std::max(6 * k, m + n));
        karatsuba(z.to_span(), x0, y0);

        z.limbs.resize(m + n);
        clear(z.to_span(2 * k));

        if (k < n || m != n) {
            x0 = bignum::norm(x0);
            CSPAN y1 = {y.p + k, y.n - k};

            nat t = mul(x0, y1);
            addAt(z.to_span(), t.to_cspan(), k);

            y0 = bignum::norm(y0);
            for (size_t i = k; i < x.n; i += k) {
                CSPAN xi = {x.p + i, std::min(k, x.n - i)};
                xi = bignum::norm(xi);
                t = mul(xi, y0);

                addAt(z.to_span(), t.to_cspan(), i);
                t = mul(xi, y1);
                addAt(z.to_span(), t.to_cspan(), i + k);
            }
        }
        z.norm();
        return z;
    }
    nat operator*(const nat &that) const {
        return mul(to_cspan(), that.to_cspan());
    }

    static nat divW(CSPAN x, WORD y, WORD *r) {
        size_t m = x.n;
        if (y == 0) throw 0;
        if (y == 1 || m == 0) return nat(x);

        nat z; z.limbs.resize(m);
        divWVW(z.to_span(), 0, x.p, y, r);
        z.norm();
        return z;
    }

    static nat divLarge(CSPAN uIn, CSPAN vIn, nat *r) {
        size_t n = vIn.n,
               m = uIn.n - n;
        size_t shift = nlz(vIn.p[n - 1]);

        nat nv; nv.make_size(n);
        SPAN v = nv.to_span();
        WORD c;
        shlVU(v, vIn.p, shift, &c);

        nat nu; nu.make_size(uIn.n + 1);
        SPAN u = nu.to_span();
        shlVU({u.p, uIn.n}, uIn.p, shift, &u.p[uIn.n]);

        nat nq; nq.make_size(m + 1);
        SPAN q = nq.to_span();
        nat nqhatv; nqhatv.make_size(n + 1);
        SPAN qhatv = nqhatv.to_span();

        WORD vn1 = v.p[n - 1];

        for (size_t j = m; ~j;--j) {
            WORD qhat = WMASK;
            WORD ujn = u.p[j + n];
            if (ujn != vn1) {
                WORD rhat;
                divWW(ujn, u.p[j + n - 1], vn1, &qhat, &rhat);

                WORD vn2 = v.p[n - 2];
                WORD x1, x2;
                mulWW(qhat, vn2, &x1, &x2);

                WORD ujn2 = u.p[j + n - 2];
                while (greaterThan(x1, x2, rhat, ujn2)) {
                    qhat--;
                    WORD prevRhat = rhat;
                    rhat += vn1;

                    if (rhat < prevRhat) break;
                    mulWW(qhat, vn2, &x1, &x2);
                }
            }

            mulAddVWW({qhatv.p, n}, v.p, qhat, 0, &qhatv.p[n]);
            WORD c;
            subVV(nu.to_span(j, qhatv.n), u.p + j, qhatv.p, &c);
            if (c) {
                addVV(nu.to_span(j, n), u.p + j, v.p, &c);
                u.p[j + n] += c;
                --qhat;
            }
            q.p[j] = qhat;
        }

        q = bignum::norm(q);
        shrVU(u, u.p, shift, &c);
        u = bignum::norm(u);
        *r = nat(u.to_cspan());
        return nat(q.to_cspan());
    }

    static nat div(CSPAN u, CSPAN v, nat *r) {
        if (v.n == 0) throw 0;
        if (bignum::cmp(u, v) < 0) {
            *r = nat(u);
            return nat();
        }
        if (v.n == 1) {
            WORD r2;
            nat q = divW(u, v.p[0], &r2);
            *r = nat(r2);
            return q;
        }

        return divLarge(u, v, r);
    }
    static nat sqr(nat x) {
        // TODO: optimization
        return mul(x.to_cspan(), x.to_cspan());
    }
    nat operator /(const nat &that) const {
        nat r;
        return div(to_cspan(), that.to_cspan(), &r);
    }
    nat operator %(const nat &that) const {
        nat r;
        div(to_cspan(), that.to_cspan(), &r);
        return r;
    }
    static void div(const nat x, const nat y, nat *q, nat *r) {
        *q = div(x.to_cspan(), y.to_cspan(), r);
    }
    static nat expW(nat x, WORD y) {
        return power(x, y, nat(1u));
    }
    size_t bitLen() const {
        size_t i = nlimb() - 1;
        if (nlimb()) return i * WBITS + bignum::bitLen(limbs[i]);
        return 0;
    }
};
#include <ojlibs/bignum_conv.inc>

} // namespace bignum
} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_BIGNUM_H_ */
