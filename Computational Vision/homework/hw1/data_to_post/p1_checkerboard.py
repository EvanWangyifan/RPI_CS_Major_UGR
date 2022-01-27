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
    print("Usage: %s im out_im m n" % sys.argv[0])
    print("where image-folder is a path to a folder containing images, and")
    print("blend-wgt is the blending weight between images in [0,0.5].")
    sys.exit()

im = sys.argv[1]
out_im = sys.argv[2]
m = int(sys.argv[3])
n = int(sys.argv[4])

og_image = cv2.imread(im)
# Calculate the side length of the desired square image
x_max = og_image.shape[1]  # horizontal
y_max = og_image.shape[0]  # vertical
square_side_length = min([x_max, y_max])
longer_side_length = max([x_max, y_max])

# crop the image
x1 = (x_max - square_side_length) // 2
x2 = x1 + square_side_length
y1 = (y_max - square_side_length) // 2
y2 = y1 + square_side_length
crop_img = og_image[y1:y2, x1:x2]
print("Image {} cropped at ({}, {}) and ({}, {})".format(im, y1, x1, y2 - 1, x2 - 1))

# resize
resized_img = cv2.resize(crop_img, (m, m))
print("Resized from {} to {}".format(crop_img.shape, resized_img.shape))

# generate the other 3 images
im_00 = resized_img.copy()
im_11 = resized_img[::-1, :, :].copy()  # image at 1,1 entry (upside down)
im_01 = np.invert(resized_img.copy())  # image at 0,1 entry (color intensity inverted)
im_10 = np.invert(im_11.copy())  # image at 1,0 entry (color intensity inverted for upside down)

# Do construction for 2*2
img_row0 = np.concatenate((im_00, im_01), axis=1)
img_row1 = np.concatenate((im_10, im_11), axis=1)
img_22 = np.concatenate((img_row0, img_row1), axis=0)

# construct n*n
img_final = np.tile(img_22, (n, n, 1))
cv2.imwrite(out_im, img_final)
print("The checkerboard with dimensions {} X {} was output to {}".format(img_final.shape[0], img_final.shape[0], out_im))
