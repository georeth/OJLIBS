#ifndef OJLIBS_INC_HUNGARIAN_SHORT_H_
#define OJLIBS_INC_HUNGARIAN_SHORT_H_
#include <vector>
namespace ojlibs { // TO_BE_REMOVED

template <typename W>
struct hungarian_short {
    int n;
    const std::vector<std::vector<W>> &cost;
    std::vector<bool> visit_x, visit_y;
    std::vector<W> lx, ly;
    std::vector<int> yx;
    bool dfs(int x) {
        visit_x[x] = true;
        for (int y = 0; y < n; ++y) {
            if (!visit_y[y] && lx[x] + ly[y] == cost[x][y]) {
                visit_y[y] = true;
                if (yx[y] == -1 || dfs(yx[y])) { // trick here
                    yx[y] = x; // alternate the augment path
                    return true;
                }
            }
        }
        return false;
    }
    hungarian_short(const std::vector<std::vector<W>> &cost)
        : n((int)cost.size()), cost(cost), visit_x(n), visit_y(n), lx(n), ly(n), yx(n, -1)
    {
        for (int x = 0; x < n; ++x) {
            lx[x] = cost[x][0];
            for (int y = 1; y < n; ++y)
                lx[x] = std::max(lx[x], cost[x][y]);
        }
        // a small but effective optimization (2476ms -> 1493ms)
        for (int y = 0; y < n; ++y) {
            W ans = cost[0][y] - lx[0];
            for (int x = 1; x < n; ++x)
                ans = std::max(ans, cost[x][y] - lx[x]);
            ly[y] = ans;
        }
        for (int i = 0; i < n; ++i) {
            while (!dfs(i)) {
                W delta = std::numeric_limits<W>::max();
                for (int x = 0; x < n; ++x) if (visit_x[x])
                    for (int y = 0; y < n; ++y) if (!visit_y[y])
                        delta = std::min(delta, lx[x] + ly[y] - cost[x][y]);
                for (int x = 0; x < n; ++x) if (visit_x[x]) lx[x] -= delta, visit_x[x] = false;
                for (int y = 0; y < n; ++y) if (visit_y[y]) ly[y] += delta, visit_y[y] = false;
            }
        }
    }
    W answer() {
        W ans = 0;
        for (int y = 0; y < n; ++y) ans += cost[yx[y]][y];
        return ans;
    }
};

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_HUNGARIAN_SHORT_H_ */
