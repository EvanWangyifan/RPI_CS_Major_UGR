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
if len(sys.argv) != 3:
    print("Usage: %s img-folder n" % sys.argv[0])
    sys.exit()

img_folder = sys.argv[1]
n = int(sys.argv[2])
# listing all valid filenames
img_list = []
os.chdir(img_folder)
img_list = os.listdir('./')
img_list = [name for name in img_list if 'jpg' in name.lower()]
# compute all rgb descriptors
rgb_descriptors_dict = dict()
counter = 0
for img_name in img_list:
    img = cv2.imread(img_name)
    M = img.shape[0]
    N = img.shape[1]
    s_m = M / n
    s_n = N / n
    rgb_descriptor = np.array([])
    for i in range(n):
        for j in range(n):
            clip = img[round(i * s_m):round((i + 1) * s_m), round(j * s_n):round((j + 1) * s_n)]
            avg = np.average(clip, axis=(0, 1))[::-1]  # remember to flip bgr to rgb
            rgb_descriptor = np.append(rgb_descriptor, avg)
    rgb_descriptor = (rgb_descriptor / np.linalg.norm(rgb_descriptor)) * 100  # normalization
    rgb_descriptors_dict[img_name] = rgb_descriptor


# Calculating distance
dis_output = []
for img_name1 in img_list:
    dis_to_current = []
    for img_name2 in img_list:
        if img_name1 != img_name2:
            distance = np.linalg.norm(rgb_descriptors_dict[img_name1] - rgb_descriptors_dict[img_name2])
            title = img_name1 + ' to ' + img_name2 + ': '
            dis_to_current.append((distance, title))
    dis_to_current.sort()  # finding out the closest to current location
    dis_output.append(dis_to_current[0])
dis_output.sort(key=lambda x: x[1])  # sort by alphabetical order of the title

# output first 3 lines
img_list.sort()
first_img = img_list[0]
print("Nearest distances\nFirst region: {:.3f} {:.3f} {:.3f}".format(rgb_descriptors_dict[first_img][0], rgb_descriptors_dict[first_img][1], rgb_descriptors_dict[first_img][2]))
print("Last region: {:.3f} {:.3f} {:.3f}".format(rgb_descriptors_dict[first_img][-3], rgb_descriptors_dict[first_img][-2], rgb_descriptors_dict[first_img][-1]))
# rest output
for dis_title in dis_output:
    print("{}{:.2f}".format(dis_title[1], dis_title[0]))
