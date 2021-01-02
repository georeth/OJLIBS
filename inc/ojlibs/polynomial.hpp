#ifndef OJLIBS_INC_POLYNOMIAL_H_
#define OJLIBS_INC_POLYNOMIAL_H_

#include <vector>
#include <deque>
#include <ojlibs/bit_trick.hpp>

namespace ojlibs { // TO_BE_REMOVED

constexpr int OJLIBS_CONVOLUTION_THRES = 50;

template <typename Alg, typename T = typename Alg::type>
void fft(std::vector<T> &P, int n, bool inv, const Alg &alg) {
    int sz = 1 << n;
    // bit reversal stage
    for (int i = 1, j = 0; i < sz - 1; i++) {
        // simulate reversed addition on j
        for (int s = sz; j ^= s >>= 1, ~j & s;)
            continue;
        if (i < j) {
            std::swap(P[i], P[j]);
        }
    }
    auto &&es = alg.unit_roots(n, inv);
    for (int d = 0; d < n; d++) {
        int m = 1 << d, m2 = m * 2;
        auto unit_p0 = es[d + 1];
        for (int i = 0; i < sz; i += m2) {
            auto unit = alg.from(1);
            for (int j = 0; j < m; j++) {
                auto &P1 = P[i + j + m], &P2 = P[i + j];
                auto t = alg.multiplies(unit, P1);
                P1 = alg.minus(P2, t);
                P2 = alg.plus(P2, t);
                unit = alg.multiplies(unit, unit_p0);
            }
        }
    }

    if (inv) {
        auto rec = alg.inverse(sz);
        for (int i = 0; i < sz; ++i)
            P[i] = alg.multiplies(P[i], rec);
    }
}

template <typename Alg, typename T = typename Alg::type>
std::vector<T> convolution_small(const std::vector<T> &v1, const std::vector<T> &v2, const Alg &alg = Alg()) {
    std::vector<T> ret(v1.size() + v2.size() - 1, alg.zero());
    for (size_t i = 0; i < v1.size(); ++i)
        for (size_t j = 0; j < v2.size(); ++j)
            ret[i + j] = alg.plus(ret[i + j], alg.multiplies(v1[i], v2[j]));
    return ret;
}

template <typename Alg, typename T = typename Alg::type>
std::vector<T> convolution(std::vector<T> v1, std::vector<T> v2, const Alg &alg = Alg()) {
    if (v1.size() < OJLIBS_CONVOLUTION_THRES || v2.size() < OJLIBS_CONVOLUTION_THRES)
        return convolution_small(v1, v2, alg);

    int sz = (int)(v1.size() + v2.size() - 1);
    int full = ceil_pow2(sz);
    int n = floor_log2(full);
    v1.resize(full); fft(v1, n, false, alg);
    v2.resize(full); fft(v2, n, false, alg);
    for (int i = 0; i < full; i++) {
        v1[i] = alg.multiplies(v1[i], v2[i]);
    }
    fft(v1, n, true, alg);
    v1.resize(sz);
    return v1;
}

template <typename Alg, typename T = typename Alg::type>
std::vector<T> convolution(const std::vector<std::vector<T>> &vs, const Alg &alg = Alg()) {
    using V = std::vector<T>;
    std::deque<V> deq;
    for (auto &v : vs)
        deq.push_back(v);

    while (deq.size() > 1) {
        V v1 = deq.front();
        deq.pop_front();
        V v2 = deq.front();
        deq.pop_front();

        deq.push_back(convolution(std::move(v1), std::move(v2), alg));
    }
    return deq.back();
}

template <typename Alg, typename T = typename Alg::type>
std::vector<T> poly_int(const std::vector<T> &v, const Alg &alg = Alg()) {
    if (v.size() == 0) return {};
    std::vector<T> ret(v.size() + 1);
    ret[0] = alg.zero();
    for (size_t i = 1; i < v.size(); ++i)
        ret[i + 1] = alg.multiplies(ret[i], alg.inverse(alg.from(i + 1)));
    return ret;
}

template <typename Alg, typename T = typename Alg::type>
std::vector<T> poly_diff(const std::vector<T> &v, const Alg &alg = Alg()) {
    if (v.size() == 0) return {};
    std::vector<T> ret(v.size() - 1);
    for (size_t i = 0; i < v.size() - 1; ++i)
        ret[i] = alg.multiplies(ret[i + 1], alg.from(i + 1));
    return ret;
}

template <typename Alg, typename T = typename Alg::type>
T poly_eval(const std::vector<T> &v, const T &x, const Alg &alg = Alg()) {
    T ret = alg.zero();
    T xn = alg.one();
    for (size_t i = 0; i < v.size(); ++i) {
        ret = alg.plus(ret, alg.multiplies(v[i], xn));
        xn = alg.multiplies(xn, x);
    }
    return ret;
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_POLYNOMIAL_H_ */
