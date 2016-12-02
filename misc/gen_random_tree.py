#!/usr/bin/env python3
import random
import sys
import os

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def rand_tree(k, no):
    children = []
    cur = no

    k -= 1
    no += 1
    while k:
        kk = random.randint(1, k)
        children.append(rand_tree(kk, no))
        k -= kk
        no += kk
    return (cur, children)

def print_tree(t):
    no, children = t
    for child in children:
        print(no, child[0])
        print_tree(child)

n = 10
print(n)
q = random.randint(100, 200)
tree = rand_tree(n, 1)
print_tree(tree)
print(q)

val = [False] * (n + 1)

def fill(root, v, found):
    if root[0] == v:
        found = True
    if found: val[root[0]] = True
    for c in root[1]:
        fill(c, v, found)

def empty(root, v):
    found = False
    if root[0] == v:
        found = True
    for c in root[1]:
        found = found or empty(c, v)
    if found: val[root[0]] = False
    return found

for _ in range(q):
    c = random.randint(1, 3)
    v = random.randint(1, n)
    print(c, v)
    if c == 1:
        fill(tree, v, False)
    elif c == 2:
        empty(tree, v)
    else:
        eprint(1 if val[v] else 0)
