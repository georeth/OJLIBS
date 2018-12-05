#ifndef OJLIBS_INC_GEOMETRY_H_
#define OJLIBS_INC_GEOMETRY_H_
#include <cmath>
#include <initializer_list>

// this file is different from linear.hpp:
//      small dimension (usually 2 or 3)
//      dot product, cross product, directed area

namespace ojlibs { // TO_BE_REMOVED

template <int Dim, typename T>
struct vec {
    T coord[Dim] = {};

    vec() { }
    vec(std::initializer_list<T> il) { 
        if (il.size() != Dim) throw 0;
        int i = 0;
        for (T v : il) {
            coord[i] = v;
            ++i;
        }
    }
    T operator[](int i) const { return coord[i]; }
    T &operator[](int i) { return coord[i]; }
    T x() const { return coord[0]; }
    T &x() { return coord[0]; }
    T y() const { return coord[1]; }
    T &y() { return coord[1]; }
    T z() const { return coord[2]; }
    T &z() { return coord[2]; }
    double norm() {
        double ret = coord[0];
        for (int i = 1; i < Dim; ++i)
            ret = hypot(ret, coord[i]);
        return ret;
    }
    template <typename R = T>
    R sq_norm() {
        R ret = 0;
        for (int i = 0; i < Dim; ++i)
            ret += static_cast<R>(coord[i]) * coord[i];
        return ret;
    }
    vec &operator+=(const vec &that) {
        for (int i = 0; i < Dim; ++i)
            coord[i] += that[i];
        return *this;
    }
    vec operator+(const vec &that) const {
        vec ret = *this;
        ret += that;
        return ret;
    }
    vec &operator-=(const vec &that) {
        for (int i = 0; i < Dim; ++i)
            coord[i] -= that[i];
        return *this;
    }
    vec operator-(const vec &that) const {
        vec ret = *this;
        ret -= that;
        return ret;
    }
    vec &operator*=(T scale) {
        for (int i = 0; i < Dim; ++i)
            coord[i] *= scale;
        return *this;
    }
    vec operator*(T scale) const {
        vec ret = *this;
        ret *= scale;
        return ret;
    }
    vec &operator/=(T scale) {
        for (int i = 0; i < Dim; ++i)
            coord[i] /= scale;
        return *this;
    }
    vec operator/(T scale) const {
        vec ret = *this;
        ret /= scale;
        return ret;
    }
    bool operator==(const vec &that) const {
        for (int i = 0; i < Dim; ++i)
            if (coord[i] != that[i]) return false;
        return true;
    }
    bool operator!=(const vec &that) const {
        return !(*this == that);
    }
};

template <typename T>
using vec2 = vec<2, T>;

template <typename T>
using vec3 = vec<3, T>;

template <typename R, int Dim, typename T>
R dot(const vec<Dim, T> &v1, const vec<Dim, T> &v2, R init) {
    for (int i = 0; i < Dim; ++i)
        init += static_cast<R>(v1[i]) * v2[i];
    return init;
}
template <typename T>
vec3<T> cross(const vec3<T> &v1, const vec3<T> &v2) {
    vec3<T> ret;
    ret.x() = v1.y() * v2.z() - v2.y() * v1.z();
    ret.y() = v1.z() * v2.x() - v2.z() * v1.x();
    ret.z() = v1.x() * v2.y() - v2.x() * v1.y();
    return ret;
}
template <typename R, typename T>
R directed_area(const vec2<T> &v1, const vec2<T> &v2, R dummy) {
    // <O, v1, v2>
    return static_cast<R>(v1.x()) * v2.y() - static_cast<R>(v2.x()) * v1.y();
}
template <typename R, typename T>
R directed_area(const vec2<T> &v1, const vec2<T> &v2, const vec2<T> &v3, R dummy) {
    // <v1, v2, v3>
    vec2<T> n2 = v2 - v1;
    vec2<T> n3 = v3 - v1;
    return directed_area(n2, n3, dummy);
}


} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_GEOMETRY_H_ */
