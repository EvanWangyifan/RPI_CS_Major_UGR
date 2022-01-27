import cv2
import numpy as np
from matplotlib import pyplot as plt
import matplotlib

import os
import random
import sys

"""
Handle the command-line arguments
"""
if len(sys.argv) != 4:
    print("Usage: %s in_img out_img dir" % sys.argv[0])
    sys.exit()

in_img = sys.argv[1]
out_img_filename = sys.argv[2]
dir = sys.argv[3]

img = cv2.imread(in_img)
M, N = img.shape[0:2]
# horizontal distance multipliers
d = np.arange(1, N + 1)
d = np.tile(d, (M, 1))
d_left = 1 - (d - 1) / (N - 1)
d_right = np.flip(d_left, 1)
# vertical distance multipliers
d = np.arange(1, M + 1)
d = np.tile(d, (N, 1))
d_top = 1 - (d - 1) / (M - 1)
d_top = np.transpose(d_top)
d_bottom = np.flip(d_top, 0)

# center distance multiplier
center_x, center_y = (M // 2, N // 2)
row = np.arange(center_x, center_x - M, -1)
col = np.arange(center_y, center_y - N, -1)
row = row.reshape(-1, 1)  # make M vertical
row = np.tile(row, (1, N))
col = np.tile(col, (M, 1))
d_center = np.sqrt(np.square(row) + np.square(col))  # get square root
d_center = 1 - (d_center / np.sqrt(np.square(center_y) + np.square(center_x)))  # normalize
np.set_printoptions(precision = 3)

# decide what is the direction
current_multiplier = None
if dir == "left":
    current_multiplier = d_left
elif dir == "right":
    current_multiplier = d_right
elif dir == "top":
    current_multiplier = d_top
elif dir == "bottom":
    current_multiplier = d_bottom
elif dir == "center":
    current_multiplier = d_center

# add multiplier and img
out_img = img.copy()
for i in range(len(current_multiplier)):
    for j in range(len(current_multiplier[i])):
        out_img[i][j] = img[i][j] * current_multiplier[i][j]

np.set_printoptions(precision=3)
# cv2.imshow('p3', out_img)
# cv2.waitKey(0)

sidebyside_img = np.concatenate((img, out_img), axis=1)
cv2.imwrite(out_img_filename, sidebyside_img)

for i in [0, M // 2, M - 1]:
    for j in [0, N // 2, N - 1]:
        print("({},{}) {:.3f}".format(i, j, current_multiplier[i][j]))
