using WORD = uint32_t;
using DWORD = uint64_t;
using VEC = std::vector<WORD>;
struct CSPAN { const WORD *p; size_t n;
//     void print(const char *h) {
//         fprintf(stderr, "%s: ", h);
//         for (int i = 0; i < n; ++i)
//             fprintf(stderr, "%x ", p[i]);
//         fprintf(stderr, "\n");
//     }
};
struct SPAN { WORD *p; size_t n; CSPAN to_cspan() const { return {p, n}; }
//     void print(const char *h) { to_cspan().print(h); }
};
static const size_t WBYTES = 4;
static const size_t WBITS = 32;
static const WORD WMASK = ~WORD(0);

// *c is INPUT and OUTPUT argument in WORD_ADD WORD_SUB
// in all other place, c is INPUT ONLY, *c is OUTPUT ONLY!!
WORD WORD_ADD(WORD w1, WORD w2, WORD *c) {
    // unsigned overflow is treated correctly by compiler
    WORD sum = w1 + w2 + *c;
    if (sum <= w1 && (w2 || *c)) *c = 1;
    else *c = 0;

    return sum;
}
WORD WORD_SUB(WORD w1, WORD w2, WORD *b) {
    // unsigned overflow is treated correctly by compiler
    WORD diff = w1 - w2 - *b;
    if (diff >= w1 && (w2 || *b)) *b = 1;
    else *b = 0;

    return diff;
}
void mulWW(WORD x, WORD y, WORD *z1, WORD *z0) {
    DWORD z = DWORD(x) * y;
    *z0 = WORD(z);
    *z1 = WORD(z >> WBITS);
}
void mulAddWWC(WORD x, WORD y, WORD c, WORD *z1, WORD *z0) {
    mulWW(x, y, z1, z0);
    WORD cc = 0;
    *z0 = WORD_ADD(*z0, c, &cc);
    *z1 += cc;
}
void divWW(WORD u1, WORD u0, WORD v, WORD *q, WORD *r) {
    DWORD u = DWORD(u1) << WBITS | u0;
    *q = u / v;
    *r = u % v;
}
void addVV(SPAN zp, const WORD *x, const WORD *y, WORD *c) {
    *c = 0;
    for (size_t i = 0; i < zp.n; ++i)
        zp.p[i] = WORD_ADD(x[i], y[i], c);
}
void subVV(SPAN zp, const WORD *x, const WORD *y, WORD *b) {
    *b = 0;
    for (size_t i = 0; i < zp.n; ++i)
        zp.p[i] = WORD_SUB(x[i], y[i], b);
}
void addVW(SPAN zp, const WORD *x, WORD y, WORD *c) {
    *c = y;
    for (size_t i = 0; i < zp.n; ++i)
        zp.p[i] = WORD_ADD(x[i], 0, c);
}
void addVWlarge(SPAN zp, const WORD *x, WORD y, WORD *c) {
    addVW(zp, x, y, c);
}
void subVW(SPAN zp, const WORD *x, WORD y, WORD *b) {
    *b = y;
    for (size_t i = 0; i < zp.n; ++i)
        zp.p[i] = WORD_SUB(x[i], 0, b);
}
void subVWlarge(SPAN zp, const WORD *x, WORD y, WORD *b) {
    subVW(zp, x, y, b);
}

void shlVU(SPAN zp, const WORD *x, size_t s, WORD *c) {
    if (s == 0) {
        std::copy(x, x + zp.n, zp.p);
        return;
    }
    if (zp.n == 0) return;

    s &= WBITS - 1;
    size_t shat = WBITS - s;
    shat &= WBITS - 1;

    *c = x[zp.n - 1] >> shat;
    for (size_t i = zp.n - 1; i > 0; i--)
        zp.p[i] = x[i] << s | x[i - 1] >> shat;
    zp.p[0] = x[0] << s;
}
void shrVU(SPAN zp, const WORD *x, size_t s, WORD *c) {
    if (s == 0) {
        std::copy(x, x + zp.n, zp.p);
        return;
    }
    if (zp.n == 0) return;

    s &= WBITS - 1;
    size_t shat = WBITS - s;
    shat &= WBITS - 1;

    *c = x[0] << shat;
    for (size_t i = 0; i < zp.n - 1; ++i)
        zp.p[i] = x[i]>>s | x[i+1]<<shat;
    zp.p[zp.n - 1] = x[zp.n - 1] >> s;
}

void mulAddVWW(SPAN zp, const WORD *x, WORD y, WORD r, WORD *c) {
    *c = r;
    for (size_t i = 0; i < zp.n; ++i)
        mulAddWWC(x[i], y, *c, c, &zp.p[i]);
}

void addMulVVW(SPAN zp, const WORD *x, WORD y, WORD *c) {
    *c = 0;
    for (size_t i = 0; i < zp.n; ++i) {
        WORD z1, z0;
        mulAddWWC(x[i], y, zp.p[i], &z1, &z0);
        zp.p[i] = WORD_ADD(z0, 0, c);
        *c += z1;
    }
}
void divWVW(SPAN zp, WORD xn, const WORD *x, WORD y, WORD *r) {
    *r = xn;
    for (size_t i = zp.n; i-- > 0;)
        divWW(*r, x[i], y, &zp.p[i], r);
}
static size_t len4tab[16] = {
    0, 1, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4};
size_t bitLen(WORD x) {
    size_t n = 0;
    if (x >= WORD(1)<<16) {
        x >>= 16;
        n = 16;
    }
    if (x >= WORD(1) << 8) {
        x >>= 8;
        n += 8;
    }
    if (x >= WORD(1) << 4) {
        x >>= 4;
        n += 4;
    }
    return n + len4tab[x];
}
size_t nlz(WORD x) {
    return WBITS - bitLen(x);
}
bool greaterThan(WORD x1, WORD x0, WORD y1, WORD y0) {
    return x1 > y1 || (x1 == y1 && x0 > y0);
}
