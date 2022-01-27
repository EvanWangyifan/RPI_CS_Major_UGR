import cv2
import numpy as np
from matplotlib import pyplot as plt
import matplotlib

import os
import random
import sys

blend_wgt = 0.5
print("OpenCV version:", cv2.__version__)
print("NumPy version:", np.__version__)
print("MatPlotLib version:", matplotlib.__version__)

"""
Handle the command-line arguments
"""
if len(sys.argv) != 3:
    print("Usage: %s in_image out_img" % sys.argv[0])
    sys.exit()

img1 = cv2.imread(sys.argv[1]).copy()
img2 = cv2.resize(img1, (img1.shape[1]//2, img1.shape[0]//2))
img3 = cv2.resize(img2, (img1.shape[1]//4, img1.shape[0]//4))

img2[img3.shape[0]//2:(img3.shape[0]//2)+img3.shape[0], img3.shape[1]//2:(img3.shape[1]) + img3.shape[1]//2, :] = img3

img1[img2.shape[0]//2:(img2.shape[0]//2)+img2.shape[0], img2.shape[1]//2:(img2.shape[1]) + img2.shape[1]//2, :] = img2

img1 = img1[::,::,::-1]
img2 = img2[::,::,::-1]
img3 = img3[::,::,::-1]

plt.imshow(img1)
plt.show()
img1 = img1[::,::,::-1]
cv2.imwrite(sys.argv[2],img1)


