#ifndef OJLIBS_INC_GEOMETRY_H_
#define OJLIBS_INC_GEOMETRY_H_
#include <cmath>
#include <vector>
#include <utility>

namespace ojlibs { // TO_BE_REMOVED

// P - point
// L - line
// S - segment
// R - rectangle
// C - circle
// I - interval
// G - polygon
// is - intersect

static const double EPS = 1e-8;

static inline int sign(double x) {
    return x < -EPS ? -1 : x > EPS;
}

// POINT
struct P {
    double x, y;
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

bool isMiddle(double a, double m, double b) {
    return sign(a - m) == 0 || sign(b - m) == 0 || (a < m != b < m);
}
bool isMiddle(const P &p1, const P &q, const P &p2) {
    // q is inside the rectangle?
    return isMiddle(p1.x, q.x, p2.x) && isMiddle(p1.y, q.y, p2.y);
}

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
    L(P p1, P p2): ps{p1, p2} { }
    P &operator[](int i) { return ps[i]; }
    P const &operator[](int i) const { return ps[i]; }
    P dir() const { return ps[1] - ps[0]; }
    double alpha() const { return dir().alpha(); }
    double abs() const { return dir().abs(); }
    double dist(P p) const { return ccw(ps[0], ps[1], p) / abs(); }
    bool onL(P p) const { return ccw(ps[0], ps[1], p) == 0; }
    bool onS(P p) const {
        return sign(ccw(ps[0], ps[1], p)) == 0
            && isMiddle(ps[0], p, ps[1]);
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
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);

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

// polygon
static inline double area(const vector<P> &ps) { // unsigned
    int n = (int)ps.size();
    double ans = 0;
    for (int i = 0; i < n; ++i) {
        P p1 = ps[i];
        P p2 = ps[(i + 1) % n];
        ans += det(p1, p2);
    }
    return abs(ans / 2);
}
static inline bool onG(const vector<P> &ps, const P &q, int *cycle = nullptr) {
    int n = (int)ps.size();
    double r = 0, pi = acos(0) * 2;
    for (int i = 0; i < n; ++i) {
        P p1 = ps[i];
        P p2 = ps[(i + 1) % n];

        if (L(p1, p2).onS(q)) return true;

        double a = (p2 - q).alpha() - (p1 - q).alpha();
        // the sign of fmod is strange... do not use it
        if (a > pi) a -= 2 * pi;
        else if (a < -pi) a += 2 * pi;
        r += a;
    }
    printf("r = %lf, pi = %lf\n", r, pi);
    if (cycle) *cycle = round(r / 2 / pi);
    return false;
}

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GEOMETRY_H_ */
