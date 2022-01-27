"""
Lecture 3 Exercises
CSCI 4270 / 6270
Prof. Stewart

This is the start for the problem 2 code. Please modify 
"""

import sys
import numpy as np


def read_pts(f):
    pts = []
    for line in f:
        line = line.strip().split()
        x, y = [float(v) for v in line]
        pts.append([x, y])
    return np.array(pts)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: %s pts.txt" % sys.argv[0])
        sys.exit(0)
    f = open(sys.argv[1])
    if f is None:
        print("Could not open %s" % sys.argv[1])
        sys.exit()

    pts = read_pts(f)

    
