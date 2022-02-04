"""
Start to Lecture 06 Exercise
CSCI 4270 / 6270
Prof. Stewart
"""

import sys
import numpy as np
import cv2


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: %s image" % sys.argv[0])
        sys.exit(0)

    im = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    if im is None:
        print("Could not read image %s" % sys.argv[1])
        sys.exit(0)
    
    # two-value kernel
    dx_kernel = np.array([[-0.5, 0, 0.5]])
    dy_kernel = np.array([[-0.5], [0], [0.5]])
    im_dx = cv2.filter2D(im, cv2.CV_32F, dx_kernel)
    im_dy = cv2.filter2D(im, cv2.CV_32F, dy_kernel)
    grad_mag = np.sqrt(im_dx ** 2 + im_dy ** 2)
    print('Two-valued average: {:.1f}'.format(np.average(grad_mag)))

    # Sobel kernel
    im_dx = cv2.Sobel(im, cv2.CV_32F, 1, 0) / 8
    im_dy = cv2.Sobel(im, cv2.CV_32F, 0, 1) / 8
    sobel_mag = np.sqrt(im_dx ** 2 + im_dy ** 2)
    print('Sobel average: {:.1f}'.format(np.average(sobel_mag)))

    # The maximum of the absolute value of the difference between the gradients.
    max_abs_diff = np.max(np.absolute(grad_mag - sobel_mag))
    print('Max abs diff: {:.1f}'.format(max_abs_diff))

    # The percentage of pixels (accurate to the nearest integer) where the two-valued gradient
    # magnitude is larger than the Sobel gradient magnitude.
    grad_larger_than_sobel = np.where(grad_mag>sobel_mag)
    print("Pct two-valued greater: {}".format(int(100*len(grad_larger_than_sobel[0])/(grad_mag.shape[0]*grad_mag.shape[1]))))
