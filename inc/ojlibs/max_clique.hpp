#ifndef OJLIBS_INC_MAX_CLIQUE_H_
#define OJLIBS_INC_MAX_CLIQUE_H_

// MAXIMUM CLIQUE PROBLEM (|V| ~ 100)
//
// basic algorithm: enumerate all clique by extending current clique
// pruing:
//      1. if clique + todo <= ans
//      2. if clique + max clique of unprocessed vertices <= ans
//      3. clique of [i, maxn[ <= clique of [i + 1, maxn[ + 1
//              if we found a large clique, break the loop


struct max_clique {
    static const int maxn = 105;

    int ans; // size of max found clique
    int found[maxn]; // max found clique
    int clique[maxn]; // current clique
    bool g[maxn][maxn] = {}; // graph
    // todo is sorted array of vertices that adjacent to all vertices in clique
    // and c < t for all c in clique, for all t in clique
    int todo[maxn][maxn];
    int maxc[maxn] = {}; // maxc = size of max clique in [k, maxn[, for pruning

    bool dfs(int depth, int ntodo) {
        if (ntodo == 0) {
            if (depth > ans) {
                ans = depth;
                swap(found, clique);
                return true;
            }
        } else {
            for (int i = 0; i < ntodo; ++i) {
                if (depth + ntodo - i <= ans)
                    return false; // pruning#1
                int v = todo[depth][i];
                if (depth + maxc[v] <= ans)
                    return false; // pruning#2

                int nxt = 0;
                for (int j = i + 1; j < ntodo; ++j) {
                    int w = todo[depth][j];
                    if (g[v][w]) todo[depth + 1][nxt++] = w;
                }

                clique[depth] = v;
                if (dfs(depth + 1, nxt)) return true; // pruning#3
            }
        }
        return false;
    }
    int solve() {
        ans = 0;
        for (int u = maxn - 1; u >= 0; --u) {
            int nxt = 0;
            for (int v = u + 1; v < maxn; ++v)
                if (g[u][v]) todo[1][nxt++] = v;
            dfs(1, nxt);

            maxc[u] = ans;
        }
        return ans;
    }
};

#endif /* end of include guard: OJLIBS_INC_MAX_CLIQUE_H_ */
