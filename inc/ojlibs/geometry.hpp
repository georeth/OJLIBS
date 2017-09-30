#ifndef OJLIBS_INC_GEOMETRY_H_
#define OJLIBS_INC_GEOMETRY_H_
#include <cmath>
#include <vector>
#include <utility>
#include <ojlibs/util.hpp>

namespace ojlibs { // TO_BE_REMOVED

// P - point
// L - line
// S - segment
// R - rectangle
// C - circle
// I - interval
// G - polygon
// H - half-plane
// is - intersect

static const double EPS = 1e-8;

static inline int sign(double x) {
    return x < -EPS ? -1 : x > EPS;
}

// POINT
struct P {
    double x, y;
    union { int id; void *data; };
    double &operator[](int i) { return i == 0 ? x : y; }

    explicit P(double x = 0, double y = 0) : x(x), y(y) { }
    P operator+(P that) const { return P(x + that.x, y + that.y); }
    P operator-(P that) const { return P(x - that.x, y - that.y); }
    P operator*(double scale) const { return P(x * scale, y * scale); }
    P operator/(double scale) const { return P(x / scale, y / scale); }
    bool operator<(P that) const {
        // not strict weak order, but works fine TO_BE_REMOVED
        int c = sign(x - that.x);
        if (c) return c < 0;
        return sign(y - that.y) < 0;
    }
    double abs2() const { return x * x + y * y; }
    double abs() const { return hypot(x, y); }
    double alpha() const { return atan2(y, x); }
    P rot90() const { return P(-y, x); }
    P unit() const { return *this / abs(); }
};

bool is_middle(double a, double m, double b) {
    return sign(a - m) == 0 || sign(b - m) == 0 || (a < m != b < m);
}
bool is_middle(const P &p1, const P &q, const P &p2) {
    // q is inside the rectangle?
    return is_middle(p1.x, q.x, p2.x) && is_middle(p1.y, q.y, p2.y);
}

/*
static inline double abs(const P &p1, const P &p2) {
    return hypot(p1.x - p2.x, p1.y - p2.y);
}
*/
static inline double dot(const P &p1, const P &p2) {
    return p1.x * p2.x + p1.y * p2.y;
}
static inline double det(const P &p1, const P &p2) {
    return p1.x * p2.y - p1.y * p2.x;
}
static inline double ccw(const P &p1, const P &p2, const P &p3) { // cross
    return det(p2 - p1, p3 - p1);
}

// LINE

// line or segment (ps[0] -> ps[1])
// or half-plane (left hand side / CCW > 0)
struct L { 
    P ps[2];
    L() { }
    L(const P &p1, const P &p2): ps{p1, p2} { }
    P &operator[](int i) { return ps[i]; }
    P const &operator[](int i) const { return ps[i]; }
    P dir() const { return ps[1] - ps[0]; }
    double alpha() const { return dir().alpha(); }
    double abs() const { return dir().abs(); }
    double dist(const P &p) const { return ccw(ps[0], ps[1], p) / abs(); }
    bool onL(const P &p) const { return ccw(ps[0], ps[1], p) == 0; }
    bool onS(const P &p) const {
        return sign(ccw(ps[0], ps[1], p)) == 0
            && is_middle(ps[0], p, ps[1]);
    }
};
static inline double ccw(const L &l, const P &p3) { // cross
    return ccw(l[0], l[1], p3);
}
static inline P isLL(const P &p1, const P &p2, const P &q1, const P &q2) {
    double a1 = ccw(q1, q2, p1), a2 = -ccw(q1, q2, p2);
    return (p1 * a2 + p2 * a1) / (a1 + a2);
}
static inline P isLL(const L &l1, const L &l2) {
    return isLL(l1[0], l1[1], l2[0], l2[1]);
}
static inline bool isII(double l1, double r1, double l2, double r2) {
    if (l1 > r1) std::swap(l1, r1);
    if (l2 > r2) std::swap(l2, r2);

    return !(sign(l2 - r1) > 0 || sign(l1 - r2) > 0);
}
static inline bool isSS(const P &p1, const P &p2, const P &q1, const P &q2) {
    // isII is necessary: (0,1)-(0,2) (0,3)-(0,4)
    return isII(p1.x, p2.x, q1.x, q2.x) && isII(p1.y, p2.y, q1.y, q2.y)
        && sign(ccw(q1, q2, p1)) * sign(ccw(q1, q2, p2)) <= 0
        && sign(ccw(p1, p2, q1)) * sign(ccw(p1, p2, q2)) <= 0;
}
static inline bool isSS(const L &l1, const L &l2) {
    return isSS(l1[0], l1[1], l2[0], l2[1]);
}
static inline P proj(const P &p1, const P &p2, const P &q) {
    P dir = p2 - p1;
    return p1 + dir * dot(dir, q - p1) / dir.abs2();
}
static inline P proj(const L &l, const P &q) {
    return proj(l[0], l[1], q);
}
static inline P reflect(const P &p1, const P &p2, const P &q) {
    return proj(p1, p2, q) * 2 - q;
}
static inline P reflect(const L &l, const P &q) {
    return proj(l[0], l[1], q) * 2 - q;
}

// POLYGON
static inline double area(const std::vector<P> &ps) { // signed
    int n = (int)ps.size();
    double ans = 0;
    for (int i = 0; i < n; ++i) {
        const P &p1 = ps[i];
        const P &p2 = ps[(i + 1) % n];
        ans += det(p1, p2);
    }
    return ans / 2;
}
static inline bool onG(const std::vector<P> &ps, const P &q, int *cycle = nullptr) {
    int n = (int)ps.size();
    double r = 0, pi = acos(0) * 2;
    for (int i = 0; i < n; ++i) {
        const P &p1 = ps[i];
        const P &p2 = ps[(i + 1) % n];

        if (L(p1, p2).onS(q)) return true;

        double a = (p2 - q).alpha() - (p1 - q).alpha();
        // the sign of fmod is strange... do not use it
        if (a > pi) a -= 2 * pi;
        else if (a < -pi) a += 2 * pi;
        r += a;
    }
    printf("r = %lf, pi = %lf\n", r, pi);
    if (cycle) *cycle = (int)std::round(r / 2 / pi);
    return false;
}

// Andrew's monotone chain
//
// may return duplicated points if non-strict and all points are on a line
static inline std::vector<P> convex_hull(std::vector<P> ps, int strict = 1) { // make a copy of ps
    int n = (int)ps.size();
    std::vector<P> conv(n * 2);
    if (n <= 1) return ps;

    sort(ps.begin(), ps.end());

    int k = 0;
    for (int i = 0; i < n; conv[k++] = ps[i++])
        while (k >= 2 && sign(ccw(conv[k - 2], conv[k - 1], ps[i])) < strict) --k;
    for (int i = n - 2; i >= 0; conv[k++] = ps[i--])
        while (k >= 2 && sign(ccw(conv[k - 2], conv[k - 1], ps[i])) < strict) --k;
    conv.resize(k - 1);
    return conv;
}

static inline double convex_diameter(const std::vector<P> &ps) {
    int i = 0, j = 0, n = (int)ps.size();
    if (n <= 1) return 0;

    double ans = 0;
    for (; i < n; ++i) {
        while ((ps[(j + 1) % n] - ps[i]).abs() > (ps[j] - ps[i]).abs())
            j = (j + 1) % n;
        chmax(ans, (ps[j] - ps[i]).abs());
    }
    return ans;
}

static inline L closest_point_pair(const std::vector<P> &ps, int b, int e, bool sorted = false) {
    double ans = HUGE_VAL;
    L seg;
    if (e - b < 6) {
        for (int i = b; i < e; ++i) for (int j = b; j < i; ++j)
            if (chmin(ans, (ps[i] - ps[j]).abs()))
                seg = L(ps[i], ps[j]);
        return seg;
    }
    if (!sorted) {
        std::vector<P> pcopy = ps;
        sort(pcopy.begin() + b, pcopy.begin() + e);
        return closest_point_pair(pcopy, b, e, true);
    }

    // divide
    int m = (b + e) / 2;
    L s1 = closest_point_pair(ps, b, m, true);
    L s2 = closest_point_pair(ps, m, e, true);
    if (chmin(ans, s1.abs())) seg = s1;
    if (chmin(ans, s2.abs())) seg = s2;

    // merge
    double x = ps[m].x;
    std::vector<P> qs;
    for (int i = b; i < e; ++i) {
        if (std::abs(ps[i].x - x) < ans) qs.push_back(ps[i]);
    }
    sort(qs.begin(), qs.end(), [](const P &p1, const P &p2) { return p1.y < p2.y; });
    int qn = (int)qs.size();
    for (int i = 0; i < qn; ++i) {
        for (int j = i + 1; j < qn && qs[j].y <= qs[i].y + ans; ++j) {
            if (chmin(ans, (qs[i] - qs[j]).abs()))
                seg = L(qs[i], qs[j]);
        }
    }
    return seg;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GEOMETRY_H_ */
