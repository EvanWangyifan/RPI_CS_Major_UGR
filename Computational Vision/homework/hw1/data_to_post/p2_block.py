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
if len(sys.argv) != 5:
    print("Usage: %s img m n b" % sys.argv[0])
    sys.exit()

img = sys.argv[1]
m = int(sys.argv[2])  # target number of row
n = int(sys.argv[3])  # target number of row
b = int(sys.argv[4])  # single block size

og_image_int = cv2.imread(img, cv2.IMREAD_GRAYSCALE)
og_image = og_image_int.astype(np.float64)  # convert to float np array

M = og_image.shape[0]
N = og_image.shape[1]
s_m = M/m
s_n = N/n

# creating downsized image
all_grey_levels = []

downsized_img = np.zeros((m, n))
for i in range(m):
    for j in range(n):
        clip = og_image[round(i*s_m):round((i+1)*s_m), round(j*s_n):round((j+1)*s_n)]
        mean = np.mean(clip.flatten())
        downsized_img[i][j] = np.float64(mean)

print("Downsized images are ({}, {})".format(m, n))


# creating binary image
median_threshold = np.median(downsized_img.flatten())

binary_img = downsized_img.copy()
for i in range(m):
    for j in range(n):
        if binary_img[i][j] >= median_threshold:
            binary_img[i][j] = 255
        else:
            binary_img[i][j] = 0

# convert the average gray scale image to integer by rounding.
downsized_img_int = np.zeros_like(downsized_img)
for i in range(m):
    for j in range(n):
        downsized_img_int[i][j] = round(downsized_img[i][j])
downsized_img_int = downsized_img.astype(int)

# construct downsized block image
downsized_block_img = np.zeros((m*b, n*b))
binary_block_img = downsized_block_img.copy()
for i in range(m):
    for j in range(n):
        clip1 = downsized_block_img[round(i * b):round((i + 1) * b), round(j * b):round((j + 1) * b)]
        clip1[:] = downsized_img_int[i][j]
        clip2 = binary_block_img[round(i * b):round((i + 1) * b), round(j * b):round((j + 1) * b)]
        clip2[:] = binary_img[i][j]
print("Block images are ({}, {})".format(downsized_block_img.shape[0], downsized_block_img.shape[1]))

#  sampling average output intensity
x, y = (m // 4, n // 4)
avg = round(downsized_img[x][y], 2)
print("Average intensity at ({}, {}) is {:.2f}".format(x, y, avg))
x, y = (m // 4, 3*n // 4)
avg = round(downsized_img[x][y], 2)
print("Average intensity at ({}, {}) is {:.2f}".format(x, y, avg))
x, y = (3*m // 4, n // 4)
avg = round(downsized_img[x][y], 2)
print("Average intensity at ({}, {}) is {:.2f}".format(x, y, avg))
x, y = (3*m // 4, 3*n // 4)
avg = round(downsized_img[x][y], 2)
print("Average intensity at ({}, {}) is {:.2f}".format(x, y, avg))
print("Binary threshold: {:.2f}".format(median_threshold))

# write image
filename_out = img.split('.')
cv2.imwrite(filename_out[0]+'_g.'+filename_out[1], downsized_block_img)
cv2.imwrite(filename_out[0]+'_b.'+filename_out[1], binary_block_img)
print("Wrote image {}".format(filename_out[0]+'_g.'+filename_out[1]))
print("Wrote image {}".format(filename_out[0]+'_b.'+filename_out[1]))

cv2.imshow('p2', binary_block_img)
cv2.waitKey(0)


