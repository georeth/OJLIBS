#ifndef OJLIBS_INC_UTIL_BINARY_SEARCH_H_
#define OJLIBS_INC_UTIL_BINARY_SEARCH_H_
#include <algorithm>
#include <cstring>

namespace ojlibs { // TO_BE_REMOVED

// std::mid_point is in C++20
template <typename It, bool IsIter = false>
It midpoint_inner(It L, It R) {
    if constexpr (IsIter)
        return std::advance(L, std::distance(L, R) / 2);
    else {
        if (L > R) std::swap(L, R);
        if (L <= 0 && R >= 0) return (L + R) / 2;
        return L + (R - L) / 2;
    }
}

// we assume:
// 1) func(x) is monotone
// 2) RTrue <=> func(x) is increasing
// 3) func(L) and func(R) is not same
// if so ret is the maximum / minimum value in [L, R] s.t. func(ret) == RetTrue
//
// unlike lower_bound, we never evaluate on L or R.
// so caller should ensures 3) before invoke this function.
// reasons:
//      not well-defined if RTrue=false RetTrue=true func(L)=false
//      we don't have a iterator before vec.begin()
//      eval func(R) for iterator may be undefined behaviour if R is end()
template <typename It, typename F, bool RTrue, bool RetTrue, bool IsIter>
It bsearch_inner(It L, It R, F func) {
    if (L == R) return L;
    // if constexpr (EvalEnds) {
    //     if (RetTrue ^ RTrue) {
    //         if (RetTrue == func(R)) return R;
    //     } else if (RetTrue == func(L)) return L;
    // }
    while (L + 1 < R) {
        It mid = midpoint_inner<It, IsIter>(L, R);
        // call func(mid) instead of func(*mid) even if IsIter
        (func(mid) ^ RTrue ? L : R) = mid;
    }
    return RetTrue ^ RTrue ? L : R;
}

#define G0(t1,v1) G1(t1,f,v1,false) G1(t1,t,v1,true)
#define G1(t1,t2,v1,v2) G2(t1,t2,,v1,v2,false) G2(t1,t2,i,v1,v2,true)
// itft -> `It` is iterator, L part is true, R part is false, find last true index
#define G2(t1,t2,t3,v1,v2,v3) template <typename It, typename F> It t3##t1##t2(It L, It R, F func) { return bsearch_inner<It, F, v1, v2, v3>(L, R, func); }
G0(ft, true)
G0(tf, false)
#undef G2
#undef G1
#undef G0

// binary search for floating point numbers is tricky:
// 0) we assume IEEE 754
// 1) if we set large epsilon, the accuracy may be not enough.
//    if we set small epsilon, the loop may never stop. (e.g. nextup(1e9) - 1e9 ~ 1e-7)
// 2) double is 64-bit width, one may expect ~64 loops are enough.
//    however, the values are not evenly distributed.
//    if we need most accurate result. i.e. func(left) ^ func(right) == true.
//    start from [-max, max] to [0, nextup(0)] will take ~2100 loops.
//    200 loops is enough for most problems if not all.
// 3) the break condition can be `mid == left || mid == right`.
//    but complier may optimize that condition to `false`, assumming no rounding.
//    I don't know elegant way to turn off this optimization.
// 4) `(left + right) / 2` may lose accurary,
//    `left + (right - left) / 2` may overflow if different sign,
//    use std::midpoint() if possible.
// 5) assume `mid = left + (right - left) / 2`,
//    the break condition can be `right != std::nextafter(left, right)`.
//    it assumes `x + (nextup(nextup(x)) - x) / 2 > x`. i think it is always true.
// 6) we can use type-punning to convert double to int64_t.
//    because IEEE 754 ensures ordering is kept, integer binary search with <=64 loops can be used.
//    however, due to alias rule, using union / reinterpret_cast results in undefined behaviour.
//    although all known compilers behave as expected.
//
//    as this library claims to be standard conforming, we have to avoid UB.
//    in C++20, bit_cast<To, From>() can be used. but for now, use memcpy() to implement our version.
template <typename To, typename From>
To my_bit_cast(const From& src) {
    static_assert(sizeof(To) == sizeof(From), "wrong size for bit_cast()");
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}
template <typename F, bool RTrue, bool RetTrue>
double bsearch_double(double left, double right, F func) {
    int64_t left_idx = my_bit_cast<int64_t, double>(left);
    int64_t right_idx = my_bit_cast<int64_t, double>(right);
    assert(right > left && right_idx > left_idx);

    while (left_idx + 1 < right_idx) {
        int64_t mid_idx = midpoint_inner(left_idx, right_idx);
        double mid = my_bit_cast<double, int64_t>(mid_idx);
        (func(mid) ^ RTrue ? left_idx : right_idx) = mid_idx;
    }
    return my_bit_cast<double, int64_t>(RetTrue ^ RTrue ? left_idx : right_idx);
}

#define G0(t1,v1) G1(t1,f,v1,false) G1(t1,t,v1,true)
#define G1(t1,t2,v1,v2) template <typename F> double d##t1##t2(double L, double R, F func) { return bsearch_double<F, v1, v2>(L, R, func); }
G0(ft, true)
G0(tf, false)
#undef G1
#undef G0

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_UTIL_BINARY_SEARCH_H_
