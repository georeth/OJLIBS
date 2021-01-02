static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct divisor {
    nat bbb;
    size_t nbits, ndights;
};

static const int leafSize = 8;

std::vector<divisor> divisors(int m, WORD b, size_t ndigits, WORD bb) {
    std::vector<divisor> table;

    if (leafSize == 0 || m <= leafSize)
        return {};
    int k = 1;
    for (int words = leafSize; words < m>>1; words <<= 1)
        ++k;

    table.resize(k);
    nat larger;
    for (int i = 0; i < k; ++i) {
        if (i == 0) {
            table[0].bbb = nat::expW(nat(bb), WORD(leafSize));
            table[0].ndights = ndigits * leafSize;
        } else {
            table[i].bbb = nat::sqr(table[i - 1].bbb);
            table[i].ndights = 2 * table[i - 1].ndights;
        }
        // TODO: optimization in natconv.go
        table[i].nbits = table[i].bbb.bitLen();
    }
    return table;
}
void convertWords(nat q, char *s, size_t ns, WORD b, int ndigits, WORD bb, divisor *table, size_t nt) {
    if (table && nt) {
        // q is passed by value
        nat r;
        size_t index = nt - 1;
        while (q.nlimb() > leafSize) {
            size_t max_len = q.bitLen();
            size_t min_len = max_len >> 1;

            while (index > 0 && table[index - 1].nbits > min_len) {
                --index;
            }
            if (table[index].nbits >= max_len && table[index].bbb >= q) {
                --index;
                assert(index >= 0);
            }

            nat::div(q, table[index].bbb, &q, &r);

            size_t h = ns - table[index].ndights;
            convertWords(r, s + h, ns - h, b, ndigits, bb, table, index);
            ns = h;
        }
    }
    size_t i = ns;
    WORD r;
    while (q.nlimb()) {
        q = nat::divW(q.to_cspan(), bb, &r);
        for (size_t j = 0; j < ndigits && i > 0; ++j) {
            --i;
            s[i] = digits[r % b];
            r /= b;
        }
    }
    while (i > 0) s[--i] = '0';
}

void maxPow(WORD b, WORD &p, WORD &n) {
    p = b, n = 1;
    for (WORD max = WMASK / b; p <= max; ) {
        p *= b;
        ++n;
    }
}

std::string utoa(const nat &x, WORD base) {
    size_t i = size_t(x.bitLen() / log2(base)) + 1;

    if (x.nlimb() == 0) return "0";

    std::vector<char> s(i);
    if (base == (base & -base)) {
        size_t shift = floor_log2(base);
        WORD mask = (1 << shift) - 1;
        WORD w = x.limbs[0];
        size_t nbits = WBITS;

        for (size_t k = 1; k < x.nlimb(); ++k) {
            while (nbits >= shift) {
                --i;
                s[i] = digits[w & mask];
                w >>= shift;
                nbits -= shift;
            }
            if (nbits) {
                w |= x.limbs[k] << nbits;
                --i;
                s[i] = digits[w & mask];

                w = x.limbs[k] >> (shift - nbits);
                nbits = WBITS - (shift - nbits);
            } else {
                w = x.limbs[k];
                nbits = WBITS;
            }
        }
        while (w) {
            --i;
            s[i] = digits[w & mask];
            w >>= shift;
        }
    } else {
        WORD bb, ndigits;
        maxPow(base, bb, ndigits);

        std::vector<divisor> table = divisors((int)x.nlimb(), base, ndigits, bb);
        nat q{x};
        convertWords(q, s.data(), i, base, ndigits, bb, table.data(), table.size());
        i = 0;
        while (s[i] == '0')
            ++i;
    }
    return std::string(s.begin() + i, s.end());
}

WORD parse_digit(char ch) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if ('a' <= ch && ch <= 'z') return ch - 'a' + 10;
    if ('A' <= ch && ch <= 'Z') return ch - 'A' + 10 + 26;
    assert(false);
}

nat str_to_nat(const char *s, WORD base) {
    nat z;
    WORD bn, n;
    maxPow(base, bn, n);
    WORD di = 0, i = 0;

    for (const char *p = s; *p; ++p) {
        WORD d1 = parse_digit(*p);
        di = di * base + d1;
        if (++i == n) {
            z = nat::mulAddWW(z.to_cspan(), bn, di);
            di = 0;
            i = 0;
        }
    }
    if (i) {
        z = nat::mulAddWW(z.to_cspan(), power(base, i), di);
    }
    z.norm();
    return z;
}
