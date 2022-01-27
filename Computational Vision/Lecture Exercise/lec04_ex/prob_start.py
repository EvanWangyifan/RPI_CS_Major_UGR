"""
Lecture 4 Exercises
CSCI 4270 / 6270
Prof. Stewart

This is the start for the Lecture 4 problem.  Please modify 
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
    # split x and y
    l_x = pts[:, 0]
    l_y = pts[:, 1]
    # find ui and vi
    u_i = l_x - np.average(l_x)
    v_i = l_y - np.average(l_y)
    # construct M matrix
    M = np.array([[np.sum(np.square(u_i)), np.sum(u_i*v_i)],
                  [np.sum(u_i*v_i), np.sum(np.square(v_i))]])
    # get eigenvector as column (so should be at position 0 and 2 for flatten 2*2 matrix)
    v = np.linalg.eigh(M)[1].flatten()
    # reverse the sign for a if needed
    a = abs(v[0])
    if v[0] < 0:
        b = -1*v[2]
    # calculate c
    c = -1 * a * np.average(l_x) - 1 * b * np.average(l_y)

    print("{:.3f}".format(a))
    print("{:.3f}".format(b))
    print("{:.3f}".format(c))
