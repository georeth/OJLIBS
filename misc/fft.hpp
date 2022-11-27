#ifndef OJLIBS_INC_FFT_H_
#define OJLIBS_INC_FFT_H_

#include <complex>

namespace ojlibs { // TO_BE_REMOVED

// This implementation is adopted from ftiasch
typedef complex<double> Complex;
void FFT(Complex P[], int n, bool inv = false) {
    int sz = 1 << n;
    // bit reversal stage
    for (int i = 1, j = 0; i < sz - 1; i++) {
        // simulate reversed addition on j
        for (int s = sz; j ^= s >>= 1, ~j & s;);
        if (i < j) {
            swap(P[i], P[j]);
        }
    }
    Complex unit_p0;
    for (int d = 0; d < n; d++) {
        int m = 1 << d, m2 = m * 2;
        double p0 = M_PI / m * (inv ? -1 : 1);
        unit_p0.imag(sin(p0));
        unit_p0.real(cos(p0));
        for (int i = 0; i < sz; i += m2) {
            Complex unit = 1;
            for (int j = 0; j < m; j++) {
                Complex &P1 = P[i + j + m], &P2 = P[i + j];
                Complex t = unit * P1;
                P1 = P2 - t;
                P2 = P2 + t;
                unit = unit * unit_p0;
            }
        }
    }

    if (inv) {
        for (int i = 0; i < sz; ++i)
            P[i] /= sz;
    }
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_FFT_H_ */
