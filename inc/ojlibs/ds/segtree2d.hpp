#ifndef OJLIBS_INC_SEGTREE2D_H_
#define OJLIBS_INC_SEGTREE2D_H_

#include <vector>
#include <type_traits>
#include <ojlibs/util/bit_trick.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <typename T>
struct segtree2d_traits {
    typedef std::true_type commutative;
    static void assoc_inplace_left(T &left, const T &right) {
        left = assoc(left, right);
    }
    static void assoc_inplace_right(const T &left, T &right) {
        right = assoc(left, right);
    }
    static T assoc(const T &x, const T &y) {
        return x + y;
    }
    static std::enable_if<commutative::value>::type assoc_inplace(const T &ele, T &v) {
        ele = assoc(ele, v);
    }
};

template <typename T, typename traits_type = segtree2d_traits<T> >
struct segtree2d {
    typedef std::pair<int, int> key_t;
    std::vector<std::vector<T>> arr;
    key_t size; // size of last layer
    key_t offset;
    segtree2d(key_t n) {
        size = n;
        offset = { ceil_pow2(n.first), ceil_pow2(n.second)} ;
        arr.resize(2 * offset.first,
                std::vector<int>(2 * offset.second));
    }
    bool inbound(int np1, int np2) {
        return np1 < 2 * offset.first
            && np2 < 2 * offset.second;
    }
    void update_parent(int np1, int np2) {
            if (inbound(np1 + np1, np2) && inbound(np1 + np1 + 1, np2)) {
                arr[np1][np2] = traits_type::assoc(arr[np1 + np1][np2], arr[np1 + np1 + 1][np2]);
            } else if (inbound(np1, np2 + np2) && inbound(np1, np2 + np2 + 1)){
                arr[np1][np2] = traits_type::assoc(arr[np1][np2 + np2], arr[np1][np2 + np2 + 1]);
            }
    }
    void increase(key_t p, const T &n) {
        for (int r1 = p.first + offset.first; r1; r1 >>= 1)
        for (int r2 = p.second + offset.second; r2; r2 >>= 1)
            traits_type::assoc_inplace(arr[r1][r2], n);
    }
    void update(key_t p, const T &n) {
        for (int r1 = p.first + offset.first; r1; r1 >>= 1)
        for (int r2 = p.second + offset.second; r2; r2 >>= 1) {
            if (r1 == p.first + offset.first && r2 == p.second + offset.second) {
                arr[r1][r2] = n;
            } else {
                update_parent(r1, r2);
            }
        }
    }
    void rebuild() {
        for (int r1 = offset.first * 2 - 1; r1 > 0; --r1)
            for (int r2 = offset.second * 2 - 1; r2 > 0; --r2) {
                update_parent(r1, r2);
            }
    }
    T query_include_1(int y, int x1, int x2) {
        T ans = T();
        if (x1 > x2) return ans;
        if (x1 == x2) return arr[x1][y];
        if (x1 & 1) {
            traits_type::assoc_inplace(ans, arr[x1][y]);
            x1 = x1 / 2 + 1;
        } else {
            x1 = x1 / 2;
        }
        if (~x2 & 1) {
            traits_type::assoc_inplace(ans, arr[x2][y]);
            x2 = x2 / 2 - 1;
        } else {
            x2 = x2 / 2;
        }
        return traits_type::assoc(ans, query_include_1(y, x1, x2));
    }
    T query_include_2(int x, int y1, int y2) {
        T ans = T();
        if (y1 > y2) return ans;
        if (y1 == y2) return arr[x][y1];
        if (y1 & 1) {
            traits_type::assoc_inplace(ans, arr[x][y1]);
            y1 = y1 / 2 + 1;
        } else {
            y1 = y1 / 2;
        }
        if (~y2 & 1) {
            traits_type::assoc_inplace(ans, arr[x][y2]);
            y2 = y2 / 2 - 1;
        } else {
            y2 = y2 / 2;
        }
        return traits_type::assoc(ans, query_include_2(x, y1, y2));
    }
    T query_include(key_t s, key_t t) {
        int x1 = s.first + offset.first, y1 = s.second + offset.second,
            x2 = t.first + offset.first, y2 = t.second + offset.second;
        return query_include_inner(x1, y1, x2, y2);
    }
    T query_include_inner(int x1, int y1, int x2, int y2) {
        if (x1 > x2 || y1 > y2)
            return T();
        else if (x1 == x2)
            return query_include_2(x1, y1, y2);
        else if (y1 == y2)
            return query_include_1(y1, x1, x2);

        T ans = T();
        if (x1 & 1) {
            traits_type::assoc_inplace(ans, query_include_2(x1, y1, y2));
            x1 = x1 / 2 + 1;
        } else {
            x1 = x1 / 2;
        }
        if (~x2 & 1) {
            traits_type::assoc_inplace(ans, query_include_2(x2, y1, y2));
            x2 = x2 / 2 - 1;
        } else {
            x2 = x2 / 2;
        }

        if (y1 & 1) {
            traits_type::assoc_inplace(ans, query_include_1(y1, x1, x2));
            y1 = y1 / 2 + 1;
        } else {
            y1 = y1 / 2;
        }
        if (~y2 & 1) {
            traits_type::assoc_inplace(ans, query_include_1(y2, x1, x2));
            y2 = y2 / 2 - 1;
        } else {
            y2 = y2 / 2;
        }
        return traits_type::assoc(ans, query_include_inner(x1, y1, x2, y2));
    }
    T &operator()(int x, int y) {
        return arr[offset.first + x][offset.second + y];
    }
};

} // ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_SEGTREE2D_H_ */
