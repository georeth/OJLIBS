// Given k group of indistinguishable balls, [v1, v2, ..., vk].
//      v1 + v2 + ... vk = N.
// count number of permutations, s.t. any two balls from the same group are not adjacent.
//
// -- Solution --
// For each permutation of `cnt` balls, there are `cnt + 1` gaps.
// We call a gap `good`, if the two balls are from different groups. Otherwise the gap is `bad`.
// e.g
//          R     R      G      B     B      R     G         (cnt = 7)
//     good   bad   good   good   bad   good   bad   good    (g = 5, b = 3)
//
// dp[i][b] = #{ permuation p of balls from v1, ..., vi : bad(p) = b }
// ans = dp[k][0]
//
// When we add a group (i - 1 -> i), the newly inserted group can eliminate and create bad gaps.
// Suppose the i-th group is split into `p` non-empty parts, p1 + ... + pp = vi.
// `f` parts are inserted into bad gaps, `p - f` parts are inserted into good gaps.
// Then `f` bad gaps are eliminated, and `vi - p` bad gaps are created.
//
// dp[i + 1][b - f + vi - p] += dp[i][b] * C(b, f) * C(g, p - f) * H(p, vi - p)
//      where g = v1 + v2 + ... + v_i-1 + 1 - b
//
// time O(N^3)
static const int maxn = 305;
static const int MOD = 1000000007;
factorial_list F(maxn * 2, MOD);
int dp[maxn][maxn];
int no_adjacent(const vector<int> &v) {
    int cnt = v[0];
    dp[0][cnt - 1] = 1;
    for (int i = 1; i < v.size(); ++i) { // i-th group to process
        for (int g = 0; g <= cnt - 1; ++g) { // g gaps to fill
            int c = cnt + 1 - g; // c non-gap
            for (int p = 1; p <= v[i]; ++p) { // x1 + ... + xp = v[i]
                for (int f = 0; f <= p; ++f) { // fill f gaps
                    if (g - f >= 0 && g - f + v[i] - p < maxn) {
                        ADDM_MOD(dp[i][g - f + v[i] - p], MUL_MOD(dp[i - 1][g],
                                MUL_MOD(F.H(p, v[i] - p), MUL_MOD(F.C(g, f), F.C(c, p - f)))));
                    }
                }
            }
        }
        cnt += v[i];
    }
    return dp[v.size() - 1][0];
}
