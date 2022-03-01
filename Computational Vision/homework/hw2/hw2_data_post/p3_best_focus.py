import cv2
import sys
import numpy as np
import os, glob

# cmd: python p3_best_focus.py evergreen
if __name__ == '__main__':
    img_dir = sys.argv[1]
    l_file = [img_dir+'\\'+file for file in os.listdir(img_dir)]
    l_file.sort()
    best_focus_images = []  # stored in list of tuple (square value, filename)
    for filename in l_file:
        img = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
        im_dx = cv2.Sobel(img, cv2.CV_32F, 1, 0)
        im_dy = cv2.Sobel(img, cv2.CV_32F, 0, 1)
        E = np.sum(im_dx**2+im_dy**2)/(img.shape[0]*img.shape[1])
        pretty_filename = filename.strip().split('\\')[-1]
        best_focus_images.append((E, pretty_filename))
        print("{}: {:.1f}".format(pretty_filename, E))
    best_focus_images.sort()
    print("Image {} is best focused.".format(best_focus_images[-1][1]))