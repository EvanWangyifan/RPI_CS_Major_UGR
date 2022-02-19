import cv2
import numpy as np
from matplotlib import pyplot as plt
import image_plot_utilities as ipu
import resize_img

# part 1
im_name = 'syntheticSquare.png'
im = cv2.imread(im_name, cv2.IMREAD_GRAYSCALE)
plt.gray()
plt.imshow(im)
plt.show()
# ipu.show_with_pixel_values(im)
im = resize_img.resize_img(im,800).astype(float)

# part 2
'''  Gaussian smoothing '''
sigma = 0.5
ksize = (round(4*sigma+1),round(4*sigma+1))
im_s = cv2.GaussianBlur(im.astype(np.float32), ksize, sigma)

'''  Derivative kernels '''
kx,ky = cv2.getDerivKernels(1,1,3)
kx = np.transpose(kx/2)
ky = ky/2

'''  Derivatives '''
im_dx = cv2.filter2D(im_s,-1,kx)
im_dy = cv2.filter2D(im_s,-1,ky)

''' Components of the outer product '''
im_dx_sq = im_dx * im_dx
im_dy_sq = im_dy * im_dy
im_dx_dy = im_dx * im_dy

''' Convolution of the outer product with the Gaussian kernel
    gives the summed values desired '''
h_sigma = 2*sigma
h_ksize = (round(4*h_sigma+1),round(4*h_sigma+1))
im_dx_sq = cv2.GaussianBlur(im_dx_sq, h_ksize, h_sigma)
im_dy_sq = cv2.GaussianBlur(im_dy_sq, h_ksize, h_sigma)
im_dx_dy = cv2.GaussianBlur(im_dx_dy, h_ksize, h_sigma)

''' Compute the Harris measure '''
kappa = 0.004
im_det = im_dx_sq * im_dy_sq - im_dx_dy * im_dx_dy
im_trace = im_dx_sq + im_dy_sq
im_harris = im_det - kappa * im_trace*im_trace

''' Show the gray scale image and the Harris image '''
ipu.plot_pics( [im, im_harris], 1, ['Original', 'Harris'] )


# part 3
''' Renormalize the intensities into the 0..255 range '''
i_min = np.min(im_harris)
i_max = np.max(im_harris)
print("Before normalization the minimum and maximum harris measures are",
     i_min, i_max)
im_harris = 255 * (im_harris - i_min) / (i_max-i_min)

'''
Apply non-maximum thresholding using dilation. Comparing the dilated image to the
Harris image will preserve only those locations that are peaks.
'''
max_dist = 2*sigma
kernel = np.ones((round(4*h_sigma+1), round(4*h_sigma+1)), im_harris.dtype)
im_harris_dilate = cv2.dilate(im_harris, kernel)
print("im_harris_dilate", im_harris_dilate.dtype)
print(len(np.where(im_harris < im_harris_dilate)))

im_harris[np.where(im_harris < im_harris_dilate)] = 0
print(im_harris.dtype)
print(np.max(im_harris), np.min(im_harris))

ipu.plot_pics([im_harris_dilate, cv2.dilate(im_harris, kernel)],
            1, ['Dilated Harris image', 'Harris peaks (dilated as well)'] )

'''
Get the normalized Harris measures of the peaks
'''
peak_values = im_harris[np.where(im_harris>0)]
peak_values = np.sort(peak_values, axis=None)
print("After NMS, there are %d peaks and this is %.2f of the total number of pixels"
      % (len(peak_values), len(peak_values)/im_harris.size))
print("And, the top, middle and bottom values are",
      peak_values[-1], peak_values[len(peak_values)//2], peak_values[0])

'''
Keep only those that are in the top fraction.
'''
threshold_frac = 0.25
threshold_index = int((1-threshold_frac)*len(peak_values))
threshold = peak_values[threshold_index]
print("The threshold to keep only the top %.1f percent of the peaks is %.1f"
      % (threshold_frac*100, threshold))
_, im_harris_thresh = cv2.threshold(im_harris.astype(np.uint8), threshold, 1, cv2.THRESH_BINARY)

# part 4
'''  Extract all indices '''
indices = np.where(im_harris_thresh > 0)
ys,xs = indices[0],indices[1]   # rows and columns

''' Put them into the keypoint list '''
kp_size = 4*sigma
harris_keypoints = [
    cv2.KeyPoint(float(xs[i]), float(ys[i]), kp_size)
    for i in range(len(xs))
]

print("We have", len(harris_keypoints), "keypoints.")

out_img = cv2.drawKeypoints( im.astype(np.uint8), harris_keypoints, None)
ipu.plot_pics( [out_img], 1, ['Harris keypoints shown'] )


# part 5
"""
This code shows the results of the SIFT keypoint detector.
"""
sift_alg = cv2.SIFT_create()
sift_kp = sift_alg.detect(im.astype(np.uint8),None)
print(len(sift_kp), "keypoints detected")
sift_out_img = cv2.drawKeypoints(im.astype(np.uint8), sift_kp, None)
ipu.plot_pics( [sift_out_img], 1, ['SIFT keypoints shown'] )