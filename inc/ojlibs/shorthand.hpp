#ifndef OJLIBS_INC_SHORTHAND_H_
#define OJLIBS_INC_SHORTHAND_H_

struct int_iterator {
    int i;
    explicit int_iterator(int i = 0) : i(i) { }
    int operator*() const { return i; }
    int_iterator &operator++() { ++i; return *this; }
    bool operator==(const int_iterator &that) const { return  i == that.i; }
    bool operator!=(const int_iterator &that) const { return !(*this == that); }
};
struct range {
    int b, e;
    range(int bp, int ep) : b(bp), e(ep) { if (e < b) e = b; }
    range(int e) : b(0), e(e) { }
    int_iterator begin() const { return int_iterator(b); }
    int_iterator end() const { return int_iterator(e); }
};
range nrange(int n) { return range(1, n + 1); }
range inrange(int b, int e) { return range(b, e + 1); }

template <typename T>
T &chmax(T &m, const T &v) { if (v > m) m = v; return m; }
template <typename T>
T &chmin(T &m, const T &v) { if (v < m) m = v; return m; }

template <typename T>
struct pos_checker {
    static bool check(const T &v) { return v >= 0; }
};

template <typename T, typename Checker = pos_checker<T> >
T &chmax_s(T &m, const T &v) {
    if (!Checker::check(v)) return m;
    if (!Checker::check(m)) return m = v;
    return chmax(m, v);
}
template <typename T, typename Checker = pos_checker<T> >
T &chmin_s(T &m, const T &v) {
    if (!Checker::check(v)) return m;
    if (!Checker::check(m)) return m = v;
    return chmin(m, v);
}

#endif /* end of include guard: OJLIBS_INC_SHORTHAND_H_ */
