"""
CSCI 4270/6270

This is the start to the exercise from Lecture 11.  You only need 
to write the function map_corners.
"""
import numpy as np
import sys


def get_matrix_3x3(fp):
    values = []
    count = 0
    for line in fp:
        line = [float(a) for a in line.strip().split()]
        if len(line) != 3:
            print("Need three floats per input line.")
            sys.exit(1)
        values.append(line)
        count += 1
        if count == 3:
            return np.array(values)


def map_corners(K1, R1, K2, R2):
    '''
    Start here
    '''
    # H = K2.R2.R1T.K1inverse
    H = K2.dot(R2).dot(R1.T).dot(np.linalg.inv(K1))
    # compute u2 and normalize z to 1
    u_2_upper_left = H.dot(np.array([0, 0, 1]))
    u_2_upper_right = H.dot(np.array([0, 4000, 1]))
    u_2_lower_left = H.dot(np.array([6000, 0, 1]))
    u_2_lower_right = H.dot(np.array([6000, 4000, 1]))
    u_2_upper_left /= u_2_upper_left[2]
    u_2_upper_right /= u_2_upper_right[2]
    u_2_lower_left /= u_2_lower_left[2]
    u_2_lower_right /= u_2_lower_right[2]
    # find the new upper left and lower right
    print("{} {}".format(round(min(u_2_upper_left[0], u_2_upper_right[0], u_2_lower_left[0], u_2_lower_right[0])),
                         round(min(u_2_upper_left[1], u_2_upper_right[1], u_2_lower_left[1], u_2_lower_right[1]))))
    print("{} {}".format(round(max(u_2_upper_left[0], u_2_upper_right[0], u_2_lower_left[0], u_2_lower_right[0])),
                         round(max(u_2_upper_left[1], u_2_upper_right[1], u_2_lower_left[1], u_2_lower_right[1]))))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage:", sys.argv[0], "matrices.txt")
        sys.exit(1)

    fp = open(sys.argv[1])
    if fp is None:
        print("Failed to open", sys.argv[1])
        sys.exit(1)

    K1 = get_matrix_3x3(fp)
    R1 = get_matrix_3x3(fp)
    K2 = get_matrix_3x3(fp)
    R2 = get_matrix_3x3(fp)

    map_corners(K1, R1, K2, R2)
    map_corners(K2, R2, K1, R1)
