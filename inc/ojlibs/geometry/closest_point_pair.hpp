#ifndef OJLIBS_INC_GEOMETRY_CLOSEST_POINT_PAIR_H_
#define OJLIBS_INC_GEOMETRY_CLOSEST_POINT_PAIR_H_

#include <cassert>
#include <utility>

namespace ojlibs { // TO_BE_REMOVED

template <typename T>
using indexed_vec2 = std::pair<vec2<T>, int>;

template <typename T>
std::pair<double, std::pair<int, int>> closest_point_pair_dc(const std::vector<indexed_vec2<T>> &vs, int b, int e) {
    double dLR = numeric_limits<double>::max();
    std::pair<int, int> pLR;

    if (e - b < 6) {
        for (int i = b; i < e; ++i)
            for (int j = i + 1; j < e; ++j) {
                double norm = (vs[i].first - vs[j].first).norm();
                if (norm < dLR) {
                    dLR = norm;
                    pLR = make_pair(vs[i].second, vs[j].second);
                }
            }
        return make_pair(dLR, pLR);
    }

    // partition
    int m = (b + e) / 2;
    T x = vs[m].first.x();

    std::pair<int, int> pL, pR;
    double dL, dR;

    std::tie(dL, pL) = closest_point_pair_dc(vs, b, m);
    std::tie(dR, pR) = closest_point_pair_dc(vs, m, e);

    dLR = dL;
    pLR = pL;
    if (dR < dLR) {
        dLR = dR;
        pLR = pR;
    }

    // valid L ( x <= cur.x < x + dLR , sort by Y)
    std::vector<indexed_vec2<T>> validL;
    for (int i = b; i < m; ++i)
        if (vs[i].first.x() < x + dLR)
            validL.push_back(vs[i]);
        else break;
    sort(validL.begin(), validL.end(), [](const indexed_vec2<T> &v1, const indexed_vec2<T> &v2) {
            return v1.first.y() < v2.first.y();
    });
    // valid R ( x <= cur.x < x + dLR , sort by Y)
    std::vector<indexed_vec2<T>> validR;
    for (int i = m; i < e; ++i)
        if (vs[i].first.x() < x + dLR)
            validR.push_back(vs[i]);
        else break;
    sort(validR.begin(), validR.end(), [](const indexed_vec2<T> &v1, const indexed_vec2<T> &v2) {
            return v1.first.y() < v2.first.y();
    });

    // do a sliding window minimum
    int s = 0, t = 0;
    for (int i = 0; i < validL.size(); ++i) {
        T y = validL[i].first.y();
        while (t < validR.size() && validR[t].first.y() < y + dLR) ++t;
        while (s < t && validR[s].first.y() < y - dLR) ++s;

        for (int j = s; j < t; ++j) {
            double norm = (validL[i].first - validR[j].first).norm();
            if (norm < dLR) {
                dLR = norm;
                pLR = make_pair(validL[i].second, validR[j].second);
            }
        }
    }
    return make_pair(dLR, pLR);
}

template <typename T>
std::pair<double, std::pair<int, int>> closest_point_pair(const vector<vec2<T>> &points) {
    // sort by X
    std::vector<indexed_vec2<T>> vs(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        vs[i] = make_pair(points[i], (int)i);

    sort(vs.begin(), vs.end(), [](const indexed_vec2<T> &v1, const indexed_vec2<T> &v2) {
            return v1.first.x() < v2.first.x();
    });
    return closest_point_pair_dc(vs, 0, (int)vs.size());
}

} // namespace ojlibs TO_BE_REMOVED
#endif//OJLIBS_INC_GEOMETRY_CLOSEST_POINT_PAIR_H_
