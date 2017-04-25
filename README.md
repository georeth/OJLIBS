# OJLIBS
C++ Utility library for online judging. Comforming to C++11 Standard.

Permit to use under MIT License.

Additionally, anybody can submit code that contains any part of this library on online judging platforms, if the usage can be restricted in online judging only.

(e.g. If someone can get submitted code on an OJ, and then use it outside the OJ legally according to the OJ's term of service, the usage of this library is forbidden.)

# Design Considerations

Readability first, Performance second, Source Code Size third.

Header Only.

Modern C++. Use standard library whenever possible. Depend on template heavily.

Trust compiler instead of hand-tuning.

# Components

## Intrusive Data structures
list : doubly-linked list

slist: singly-linked list

rbtree: red-black tree

treap, sbtree, splay, link-cut tree. (TODO)

## Special Data structures
Disjoint Set

Bit Index Tree (a.k.a Fenwick Tree)

Segment Tree (flavour I/II, and lazy)

## Arithmatic
Division with fixed rounding direction

GCD

LCM

Extended Euclid Algorithm

Modular Arithmatic

Chinese Remainder Theorem

## Mathematics Structure (TODO)
Permutaion, Combination, Subset.

Generator.

Mobius Inversion.

Subset Inversion.

Karatsuba, FFT, Multiprecision.

## Linear Algebra
Matrix

Arithmatic operation (naive implementation)

(P)LU decomposition

Determinant

## Computational Geometry
Vector Arithmatic (Linear, Dot, Cross, Directed area, Distance)

Geometry Transform (TODO)

Closest pair of points

## String Processing

KMP

Z-Function

Suffix Array (SA-IS algorithm : linear time construction)

AC-Automaton(TODO)

## Graph Theory
A Generic Graph Implementation

DFS / BFS

Shortest Path (Dijkstra, Bellman-Ford, Floyd-Warshall)

Strongly Connected Component

Topological Sort (on DAG)

Offline Tarjan LCA (on rooted/rootless tree)

Network Flow (Dinic)

Maximum Bipartitie Matching (Hungarian / Kuhn-Munkres)

Eular Walk (TODO : support removing edge in graph)

## Memoization
Generic memorization function without virtual function overhead

(Useful for top-down dynamic programming.)

## FAST IO
(TODO)

- mmap or read into big buffer (avoid \<cstdio\> or \<iostream\>)

- read\_int(), read\_string(), etc.

## TODO
2-SAT

snippets (dfs, tree, lca, segtree)

# TOOLS
- Code forces sample test cases crawler, and automatic validation script. (tools/cf\_test\_crawler)

- A tool that can combine source code with included header files of this library to produce a single source file to submit. (tools/ojlibs\_cpp)

- Solution Template : main.cpp, check.sh, interactive.sh and Makefile (src/)
