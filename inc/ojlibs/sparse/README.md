# Sparse Matrix / Graph Library

this part of OJLIBS is originally designed for sparse matrix and graph in sparse format.

A graph contains several components:
- Vertices V: indexed by integer {0, 1, ..., nvert - 1}
- Edges E: E is subset of (V x V), each edge has a edge_id (eid) indexed by integer {0, 1, ..., esize - 1}
- Vertex Attributes: V -> VAttr_i
- Edge Attributes: E -> EAttr_j

G = (V, E) is represented by `concept sparse` Sp (`ojlibs::csr` or `ojlibs::lil`).
VAttr and EAttr are stored in `std::vector<Attr>`.

E and EAttr is separated due to flexibility.
we can create EAttr on the fly, and specify which EAttr for our algorithm to use without boilerplate when there are multiple attributes.

for example, when apply Dijkstra algorithm. Input contains G(Sp) and Weight(EAttr). Output is Dist(VAttr), Visit(VAttr).

there is also a `ojlibs::graph` combining Sp and EAttr. it can access edge with its attributes easily.
it contains only references, so it owns no state.

every type in this sparse library is white-box, feel free to break abstraction.
i.e. `for (auto [v, eid] : g.sp.out(u)) do_something()`.

## DFS and related graph algorithms

this is a general DFS implementation:
```c++
void generic_dfs(int u) {
  visit[u] = 1;
  pre_vert(u);
  for (auto [v, e] : g.out(u))
    if (!visit[v]) {
      pre_edge(u, v, e);
      dfs(v);
      post_edge(u, v, e);
    } else if (visit[v] == 1) {
      found_back_edge(u, v, e)
    } else {
      found_fwd_cross_edge(u, v, e);
    }
  post_vert(u);
  visit[u] = 2;
}
```

issues:
- no early quit
- should skip (v, u) if (u, v) is visited in undirected graph, should not be a back-edge.

use recursive DFS on graph may cause stack overflow.
Since we target CodeForces (unlimited stack size), atcoder (unlimited) and GCJ (64MB),
we assume stack size >= 64MB.

on x64, recursive function without parameter and local variables takes 16B stack,
each parameter usually takes 8B, each local variable may take 8B (variable not used after first recursive call may also take space).
if we assume each frame is 40B, 64MB can support 1.6e5 input range, which is enough for most problems.

we can also rewrite recursive dfs into iterative one.
there are two flavor:

* I) when visit u, push all out(u) onto stack. Compared with recursive version:
  * out(u) is visited in reverse order
  * a vertex can be pushed onto stack multiple times (`max(1, in_degree)` times)
  * push time can be reduced by not pushing visited vertex v when visiting (u,v), as if (u,v) is visited first. however, it makes the code hard to read, since some non-tree edges are found earlier, the others not.
  * a unvisited vertex u will be accessed 3 times on stack. 1st: `push(u)` by its parent, 2nd: `pre_vert(u)` and push its children, 3rd: `post_vert(u)` and pop. if 3rd can be skipped, and pop at 2nd, it can outperform recursive version.
* II) when visit u, push next unvisited child onto stack
  * exactly the same behaviour with recursive version, since it's a program transform from recursion to iteration. each vertex is pushed onto stack only once.
  * iterator of children of u has to be pushed onto stack
  * stack entry is visited `1 + out_degree + 1` times: 1 for push, `out_degree` for push children, 1 for `post_vert()` and pop.

these two iterative version takes >1.4x time in micro-benchmark, so just use the recursive version, it's simple and fast.

thanks to optimizing C++ compilers, manully implemented recursive version is as fast as templated version. choose the one producing simpler code.

## network flow

## files

helper file: common.hpp
core files: csr.hpp lil.hpp graph.hpp

other files are specific algorithm implementations.
