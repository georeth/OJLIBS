#!/usr/bin/env python3
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

def rotate_martix_left(martix):
    return [list(l) for l in list(zip(*martix[:]))[::-1]]

def rotate_martix_right(martix):
    return [list(l) for l in zip(*martix[::-1])]

def rotate_martix_half(martix):
    return [list(reversed(l)) for l in reversed(a)]

def flip_martix_left_right(martix):
    martix = [list(reversed(l)) for l in martix]

def flip_martix_top_down(martix):
    martix = [list(l) for l in reversed(martix)]

direction4 = [[0, -1], [1, 0], [0, 1], [-1, 0]]
direction5 = [[0, -1], [1, 0], [0, 1], [-1, 0], [0, 0]]
direction8 = [[0, -1], [1, -1], [1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0], [-1, -1]]
direction9 = [[0, -1], [1, -1], [1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0], [-1, -1], [0, 0]]
directionK = [[1, -2], [2, -1], [2, 1], [1, 2], [-1, 2], [-2, 1], [-2, -1], [-1, -2]]

telephone = ["", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"]
morse = {"a":".-","b":"-...","c":"-.-.","d":"-..","e":".","f":"..-.","g":"--.","h":"....","i":"..","j":".---","k":"-.-","l":".-..","m":"--","n":"-.","o":"---","p":".--.","q":"--.-","r":".-.","s":"...","t":"-","u":"..-","v":"...-","w":".--","x":"-..-","y":"-.--","z":"--..","0":"-----","1":".----","2":"..---","3":"...--","4":"....-","5":".....","6":"-....","7":"--...","8":"---..","9":"----."}


