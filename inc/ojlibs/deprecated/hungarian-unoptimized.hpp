#ifndef OJLIBS_INC_HUNGARIAN_H_
#define OJLIBS_INC_HUNGARIAN_H_

namespace ojlibs { // TO_BE_REMOVED

// weighted maximum matching
// TODO: time O(V^4) now, should be O(V^3), optimize by slacky ...
// (plz use max flow for unweighted matching, much faster)
template <typename W>
struct hungarian {
    const std::vector<std::vector<W>> &cost;
    int n;
    std::vector<W> lx, ly;
    std::vector<W> xy, yx; // the matching X <-> Y

    std::vector<int> prev;
    std::vector<bool> S, T;
    void initial_labelling() {
        for (int x = 0; x < n; ++x) {
            W ans = cost[x][0];
            for (W y = 1; y < n; ++y)
                ans = std::max(ans, cost[x][y]);
            lx[x] = ans;
        }
        // a small but effective optimization (1671ms -> 1014ms)
        for (int y = 0; y < n; ++y) {
            W ans = cost[0][y] - lx[0];
            for (int x = 1; x < n; ++x)
                ans = std::max(ans, cost[x][y] - lx[x]);
            ly[y] = ans;
        }
    }
    // return a free y
    int bfs(int u) {
        fill(S.begin(), S.end(), false);
        fill(T.begin(), T.end(), false);
        // uninitialized prev is fine
        std::vector<int> q(n); // bfs queue (contains verteces from X)
        int qh = 0, qt = 0;

        q[qt++] = u;
        S[u] = true;
        while (qh < qt) { // bfs
            int x = q[qh++];
            for (int y = 0; y < n; ++y) {
                if (T[y] || lx[x] + ly[y] != cost[x][y])
                    // do not revisit y in T[y]
                    continue;
                prev[y] = x;
                if (yx[y] == -1) {
                    return y;
                } else if (!S[yx[y]]) {
                    // do not revisit x in S[x]
                    S[yx[y]] = true;
                    T[y] = true;
                    q[qt++] = yx[y];
                }
            }
        }
        return -1;
    }
    void update_labelling() {
        W delta = std::numeric_limits<W>::max();
        for (int x = 0; x < n; ++x) {
            if (!S[x]) continue;
            for (int y = 0; y < n; ++y)
                if (!T[y]) delta = std::min(delta, lx[x] + ly[y] - cost[x][y]);
        }
        for (int i = 0; i < n; ++i) {
            if (S[i]) lx[i] -= delta;
            if (T[i]) ly[i] += delta;
        }
    }
    void augment(int free_y) {
        int y = free_y;
        while (y != -1) {
            int x = prev[y];
            int next_y = xy[x];
            yx[y] = x;
            xy[x] = y;

            y = next_y;
        }
    }
    hungarian(const std::vector<std::vector<W>> &cost) 
        : cost(cost), n(cost.size()), lx(n), ly(n),
          xy(n, -1), yx(n, -1), prev(n, -1), S(n), T(n)
    {
        initial_labelling();
        int matched = 0;
        // improve label or matching
        while (matched < n) {
            int u;
            for (u = 0; u < n; ++u) {
                if (xy[u] == -1) break;
            }
            int free_y = bfs(u);
            if (free_y == -1) {
                update_labelling();
                continue;
            }
            augment(free_y);
            ++matched;
        }
    }
    W answer() {
        W ans = 0;
        for (int x = 0; x < n; ++x) {
            ans += cost[x][xy[x]];
        }
        return ans;
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_HUNGARIAN_H_ */
