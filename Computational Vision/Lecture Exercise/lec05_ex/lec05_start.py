"""
Lecture 5 Exercises
CSCI 4270 / 6270
Prof. Stewart

This is the start for the Lecture 5 problem.  Please modify
"""

import sys
import numpy as np


def read_matrices(f):
    """
    Get a list of 3x3 matrics from f.  Blank lines are allowed, but
    each line of numbers must have exactly three floats and nothing else.
    """
    matrices = []
    pts = []

    for line in f:
        line = line.strip().split()
        if len(line) == 0:
            continue
        line = [float(v) for v in line]
        assert(len(line) == 3)
        pts.append(line)
        if len(pts) == 3:
            h = np.array(pts)
            pts = []
            h /= h[2, 2]
            matrices.append(h)

    assert(len(pts) == 0)  # No extra values
    return matrices


def is_equal(a, b):
    return abs(a-b) < 1.e-6


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: %s matrix.txt" % sys.argv[0])
        sys.exit(0)
    f = open(sys.argv[1])
    if f is None:
        print("Could not open %s" % sys.argv[1])
        sys.exit()

    matrices = read_matrices(f)

    for h in matrices:
        #  YOUR CODE GOES HERE...

        # flag to show if already defined a transformation state
        flag = False
        # check for rigid and similarity
        if h[0][0]==h[1][1] and h[0][1]==-1*h[1][0]:
            if is_equal((np.square(h[0][0])+np.square(h[1][0])), 1): # rigid has cosx and sinx
                print("rigid")
                flag = True
            elif (np.square(h[0][0])+np.square(h[1][0])) != 0:
                print("similarity")
                flag = True
        # check for affine
        if np.linalg.det(h[0:2, 0:2]) != 0 and not flag:  # A is constrained of rank 2
            print("affine")
            flag = True
        # check homogeneous
        zero_counter = 0
        for i in range(len(h)):
            if h[i, 0] == 0 and h[i,1] == 0:
                zero_counter += 1
        if zero_counter == 1 and not flag:
            print("homography")
            flag = True
        elif not flag:
            print("none")
            flag = True






