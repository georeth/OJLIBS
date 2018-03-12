#!/usr/bin/env python3
from math import *
def read_string():
    return input()

def read_strings(return_type = iter, split = None, skip = 0):
    return return_type(input().split(split)[skip:])

def read_lines(height, return_type = iter):
    return return_type(read_string() for i in range(height))

def read_number():
    return int(input())

def read_numbers(return_type = iter, skip = 0):
    return return_type(int(i) for i in input().split()[skip:])

def read_values(*types, array = None):
    line = input().split()
    result = []
    for return_type, i in zip(types, range(len(types))):
        result.append(return_type(line[i]))
    if array != None:
        array_type, array_contained = array
        result.append(array_type(array_contained(i) for i in line[len(types):]))
    return result

def read_array(item_type = int, return_type = iter, skip = 0):
    return return_type(item_type(i) for i in input().split()[skip:])

def read_martix(height, **args):
    return_type = args["return_type"] if "return_type" in args else iter
    return_type_inner = args["return_type_inner"] if "return_type_inner" in args else return_type
    return_type_outer = args["return_type_outer"] if "return_type_outer" in args else return_type
    item_type = args["item_type"] if "item_type" in args else int
    return return_type_outer(read_array(item_type = item_type, return_type = return_type_inner) for i in range(height))

def read_martix_linear(width, skip = 0, item_type = int, skiped = None):
    num = read_array(item_type = item_type, skip = skip)
    height = len(num) / width
    return [num[i * width: (i + 1) * width] for i in range(height)]

def cf2f(cf):
    h0, h1 = 0, 1
    k0, k1 = 1, 0

    for z in cf:
        h0, k0, h1, k1 = h1, k1, h0 + h1 * z, k0 + k1 * z
    return (h1, k1)

def solve(D):
    # u1 / (n1 * sq - m1)
    # = (u1 * n1 * sq + u1 * m1) / (n1 * n1 * D - m1 * m1)
    # = (u1 * n1 * sq + u1 * m1) / norm1
    # = w1 + (u1 * n1 * sq - w1 * norm + u1 * m1) / norm1
    # = w1, norm1 / (u1 * n1 * sq - w1 * norm + u1 * m1)

    # OR:
    # (n1 * sq + m1) / d1
    # = w1 + (n1 * sq - d1 * w1 + m1)
    # = w1, 1 / (n1 * sq - d1 * w1 + m1) = 1 / (n1 * sq - p1)
    # = w1, (n1 * sq + p1) / (n1 * n1 * sq  + p1 * p1)
    sq = sqrt(D)
    w0 = int(sq)

    # start from (sq + w0), then find a cycle (the cycle contains integer part)
    u0, n0, m0 = D - w0 * w0, 1, w0
    u1, n1, m1 = u0, n0, m0
    frac = []

    while True:
        w1 = int(u1 / (n1 * sq - m1))
        frac.append(w1)
        norm1 = n1 * n1 * D - m1 * m1

        u2 = norm1
        n2 = u1 * n1
        m2 = w1 * norm1 - u1 * m1
        g = gcd(u2, gcd(n2, m2))
        u1, n1, m1 = u2 // g, n2 // g, m2 // g

        if (u1, n1, m1) == (u0, n0, m0):
            break
    if len(frac) % 2 == 0:
        # we found a solution of `x2 - D y2 = 1`
        frac[0] -= w0
        return cf2f(frac)
    else:
        # we found a solution of `x2 - D y2 = -1`
        frac = frac + frac
        frac[0] -= w0
        return cf2f(frac)

def main():
    n = read_number()

    ans, m = 0, 0
    for i in range(1, n):
        w = int(sqrt(i))
        if w * w == i:
            continue
        cur, _ = solve(i)
        if cur > m:
            ans = i
            m = cur
    print(ans)

if __name__ == '__main__':
    main()
