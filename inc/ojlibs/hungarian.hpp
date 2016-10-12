#ifndef OJLIBS_INC_HUNGARIAN_H_
#define OJLIBS_INC_HUNGARIAN_H_
#include <vector>
#include <algorithm>
#include <limits>

namespace ojlibs { // TO_BE_REMOVED
// A really good article on Hungarian Algorithm (by Subhash Suri):
//      http://www.cse.ust.hk/~golin/COMP572/Notes/Matching.pdf
// but without real code. I read it many times.
// There are so many complicated implementation claimed to be time O(V^3).
// After reading them, I was not convinced. 
// So I decide to implement a clear O(V^3) version. (~100 LOC)
//
// Another very short O(V^4) Implementation at hungarian_short.hpp (~50 LOC)
// (hungarian 12.4ms vs. hungarian_short 123.8ms when |V| = 200)
//
// weighted maximum matching
// (plz use max flow for unweighted matching, it's much faster)
template <typename W>
struct hungarian {
    const std::vector<std::vector<W>> &cost;
    int n;
    std::vector<W> lx, ly;
    std::vector<int> xy, yx; // the matching X <-> Y

    void initial_labelling() {
        for (int x = 0; x < n; ++x) {
            W ans = cost[x][0];
            for (int y = 1; y < n; ++y)
                ans = std::max(ans, cost[x][y]);
            lx[x] = ans;
        }
        // a small but effective optimization (335ms -> 261ms)
        for (int y = 0; y < n; ++y) {
            W ans = cost[0][y] - lx[0];
            for (int x = 1; x < n; ++x)
                ans = std::max(ans, cost[x][y] - lx[x]);
            ly[y] = ans;
        }
    }
    void augment(int u) {
        std::vector<bool> S(n); // T = {y : prev[y] != -1}
        // slacky[y] = min{x in S}(lx[x] + ly[y] - cost[x][y])
        std::vector<W> slacky(n, std::numeric_limits<W>::max());
        // tightyx[y] = argmin{x in s}(lx[x] + ly[y] - cost[x][y])
        std::vector<W> tightyx(n);
        std::vector<int> prev(n, -1); // alternate tree edge Y -> X
        std::vector<int> q(n); // bfs queue (contains verteces from X)
        int qh = 0, qt = 0;
        q[qt++] = u;

        int y; // exit the loop with a free y. shadowing...
        while (true) {
            while (qh < qt) { // bfs
                int x = q[qh++];
                S[x] = true;
                // S changes, update slack y
                for (y = 0; y < n; ++y) if (prev[y] == -1) {
                    W w = lx[x] + ly[y] - cost[x][y];
                    if (w < slacky[y]) slacky[y] = w, tightyx[y] = x;
                    if (w == 0) {
                        // found new neighbour(X) in equality graph
                        prev[y] = x;
                        if (yx[y] == -1) goto free_y_found; // free y found, break the outer loop
                        else q[qt++] = yx[y];
                    }
                }
            }
            // cannot augment, improve labelling
            W delta = std::numeric_limits<W>::max();
            for (int y = 0; y < n; ++y) if (prev[y] == -1)
                delta = std::min(delta, slacky[y]);
            for (int i = 0; i < n; ++i) {
                if (S[i]) lx[i] -= delta;
                if (prev[i] != -1) ly[i] += delta;
            }
            // label change, update slacky
            for (y = 0; y < n; ++y) if (prev[y] == -1) {
                slacky[y] -= delta;
                if (slacky[y] == 0) {
                    // found new neighbour(X) in equality graph
                    prev[y] = tightyx[y]; // rightyx[y] -> y drop to 0
                    if (yx[y] == -1) goto free_y_found; // free y found, break the outer loop
                    else q[qt++] = yx[y];
                }
            }
            // resume the bfs loop after relabel
        }
free_y_found:
        // augment the path (alternate the augment path)
        while (y != -1) {
            int x = prev[y];
            int next_y = xy[x];
            xy[x] = y;
            yx[y] = x;
            y = next_y;
        }
    }
    hungarian(const std::vector<std::vector<W>> &cost) 
        : cost(cost), n(cost.size()), lx(n), ly(n),
          xy(n, -1), yx(n, -1)
    {
        initial_labelling();
        for (int u = 0; u < n; ++u)
            augment(u);
    }
    W answer() {
        W ans = 0;
        for (int x = 0; x < n; ++x)
            ans += cost[x][xy[x]];
        return ans;
    }
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_HUNGARIAN_H_ */
