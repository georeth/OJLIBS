# Game theory
## Impartial Games
G = (X, F).
G is game or graph.
X is states of game, or vertices of graph.
F(x) is target endpoints from x.

Win(x) : X -> Bool
Win(x) = False if all y of F(x), Win(y)
         True otherwise.

(Lose if no moves)

G is progressively bounded if every path has finite length.

### Solution
Just use the definition, recursively or by DP.

### Utility Function Is Real Number, Zero-Sum Game
1. Min-Max
2. ans[x] = Max{y in F(x)} -ans[y]


### With loops
Win, Lose, Unknown.
Label a state when all outcomes are known.

1. Label all states Unknown.
2. Count number of moves cnt[s] for each state s.
3. While exist state s, cnt[s] = 0:
4. 	If all next(s) is Win, Label s Lose
5. 	Else Label s Win
6.  	For all prev(s') = s:
7.		Decrease cnt[s']
8. Done

## Multiple Games
### Sprague-Grundy (SG) Function
sg : X -> Int
sg(x) :=  min { n >= 0 : n != sg(y) for y in F(x)}
      = mex sg(F(x))
where mex is Minimum EXclusive value.

sg(x) == 0 <=> Lose(x)

### Sum of Game
G = G1 + G2 + ... + Gk
X = X1 * X2 * ... * Xk

the current player can choose a game to play.

### Spague-Grundy Theorem
sg(x) = sg(x1) + ... + sg(xk)
