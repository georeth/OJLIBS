# Game theory
## Definitions and Classifications
number of players (2)
position, move
random move
information
perfect information
combinatorial = two-person + win or lose + perfect information + no random move
impartial = combinatorial + two players have the same move set
partizan = combinatorial + two player have different move sets


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
sg(x) = sg(x1) xor ... xor sg(xk)

## Recursive and Stochastic Games
There are two sets of parameters: expected value of states and strategy.

To get the strategy (determine the move), we need expected value of following states.
To compute the expected value, we need know the strategy.

(866C)
if we assume the root game expected value, then all expected value and strategy can be computed.
By negative feedback, we can binary search the value.
