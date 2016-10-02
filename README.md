# OJLIBS
C++ Utility library for online judging. Comforming to C++11 Standard.

Permit to use under MIT License.

Additionally, anybody can submit code that contains any part of this library on online judging platforms, if the usage can be restricted in online judging only.

(e.g. If someone can get submitted code on an OJ, and then use it outside the OJ legally according to the OJ's term of service, the usage of this library is forbidden.)

# Design Considerations

Readability First, then Performance

Header Only

Modern C++. Use standard library whenever possible

Trust compiler instead of manually tuning

# Components

## Intrusive Data structures
list : doubly-linked list

slist: singly-linked list

rbtree: red-black tree

## Special Data structures
Disjoint Set

Segment Tree

## Arithmatic
Division with fixed rounding direction

GCD

LCM

Extended Euclid Algorithm

Modular Arithmatic

Chinese Remainder Theorem

## Multiprecision
(TODO)

Big Number

## Linear Algebra
Matrix

(P)LU decomposition

## Computation Geometry
(TODO)
Vector
Geometry Transform

## Graph Theory
(TODO)

## Memorization
Generic memorization function without virtual function overhead

Useful for top-down dynamic programming.

## FAST IO
(TODO)

- mmap or read into big buffer (avoid <cstdio> or <iostream>)

- read\_int(), read\_string(), etc.

# TOOLS
- Code forces sample test cases crawler, and automatic validation script. (tools/cf\_test\_crawler)

- A tool that can combine source code with included header files of this library to produce a single source file to submit. (tools/ojlibs\_cpp)
