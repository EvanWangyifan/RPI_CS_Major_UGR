"""
Starting code for the Lecture 13 exercise.
"""

import sys
import numpy as np


def read_votes(f):
    votes = []
    for line in f:
        line = line.strip().split()
        line = [int(s) for s in line]
        votes.append(line)
    return np.array(votes)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage:", sys.argv[0], "votes.txt")
        sys.exit(0)

    f = open(sys.argv[1], 'r')
    if f is None:
        print("Failed to open", sys.argv[1])
        sys.exit(0)

    votes = read_votes(f)

    
