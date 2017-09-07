#!/usr/bin/env python3
import util

from problem import MULTILINE_OUTPUT, solution

def main():
    count = util.read_number()
    end_str = "\n" if MULTILINE_OUTPUT else " "
    for i in range(1, count + 1):
        print("Case #%d:" % i, end = end_str)
        solution()

main()
