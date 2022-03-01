
import sys

import numpy as np
import cv2

# python start.py grevys.jpg test.jpg 2 1.4 1.3
# python start.py disk.png test.png 2 1.4 1.3
if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Usage: %s in_image out_image sigma g0 g1" % sys.argv[0])
        sys.exit(0)

    im = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    if im is None:
        print("Could not read image %s" % sys.argv[1])
        sys.exit(0)
    out_name = sys.argv[2]
    sigma = int(sys.argv[3])
    g0 = float(sys.argv[4])
    g1 = float(sys.argv[5])

    # my codes start here
    # step 1: Smooth the image with a Gaussian
    ksize = (4 * sigma + 1, 4 * sigma + 1)
    im_s = cv2.GaussianBlur(im.astype(np.float32), ksize, sigma)
    # step 2 & 3
    kx, ky = cv2.getDerivKernels(1, 1, 3)
    kx = np.transpose(kx / 2)
    ky = ky / 2
    im_dx = cv2.filter2D(im_s, -1, kx)  # Ix: partial derivatives in the x directions
    im_dy = cv2.filter2D(im_s, -1, ky)  # iy: partial derivatives in the y directions
    im_gm = np.sqrt(im_dx ** 2 + im_dy ** 2)  # Ig: gradient magnitude
    # step 4: normalization
    im_gm += 0.00001
    im_dx /= im_gm
    im_dy /= im_gm
    # step 5: Encode Ig, Ix and Iy into an L*a*b image, with Ig forming L, Ix forming a and Iy forming b.
    a = im_dx * 128
    b = im_dy * 128
    # a = (im_dx + 1) * (255 / 2)  # map [-1:1] to [0:255]
    # b = (im_dy + 1) * (255 / 2)
    L = im_gm.copy()
    L[L < g0] = 0  # threshold small gradients, and scale up larger gradients.
    L[L > g1] = g1
    L = np.round(L*100/g1)
    # step 6: L*a*b
    LAB_image = np.dstack((L, a, b)).astype('float32')
    final_image_bgr = cv2.cvtColor(LAB_image, cv2.COLOR_LAB2BGR)
    final_image_bgr[LAB_image[:, :, 0] == 0] = np.array([0, 0, 0])

    # cv2.imshow("L", L)
    # cv2.imshow("a", a)
    # cv2.imshow("b", b)
    # cv2.imshow("final", final_image_bgr)
    # cv2.waitKey(0)

    final_image_bgr = cv2.convertScaleAbs(final_image_bgr, alpha=(255.0))
    cv2.imwrite(out_name, final_image_bgr)



