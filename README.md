# OJLIBS
C++ Utility library for online judging. Comforming to C++11 Standard.

# Components

## FAST IO
- mmap or read into big buffer (avoid <cstdio> or <iostream>)
- read_int(), read_string(), etc.

## Intrusive Data structures

### Design Considerations
Use intrusive containers to avoid memory allocation overhead.
No runtime checking to get best performance.

Use C subset to do basic node manipulation.
Use C++ template to implement containers.

### Data structures
list : doubly-linked list
slist: singly-linked list
rbtree: red-black tree

## Special Data structures
Union Find Set
Interval Tree

## Number Theory
GCD / LCM
Prime Number Algorithms

## Multiprecision
Big Number

## Computation Geometry
## Graph Theory
