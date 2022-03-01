import cv2
import numpy as np
import sys
from matplotlib import pyplot as plt

# python .\p3_bird_detector.py p1.jpg p2.jpg P3_out.jpg
# python .\p3_bird_detector.py n1.jpg n2.jpg P3_out.jpg
if __name__ == '__main__':
	if len(sys.argv) > 4:
		print("USAGE: python p3_bird_detector.py img1 img2 img_out")
		exit(1)
	# input img
	img1 = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE).astype(float)
	img2 = cv2.imread(sys.argv[2], cv2.IMREAD_GRAYSCALE).astype(float)
	img_out_name = sys.argv[3]
	# find diff
	im_diff = np.zeros_like(img1)
	im_diff[:int(7/9*img1.shape[0]), int(img1.shape[1] / 3):] = np.abs(img1[:int(7/9*img1.shape[0]), int(img1.shape[1] / 3):] - img2[:int(7/9*img1.shape[0]), int(img1.shape[1] / 3):])
	np.savetxt("bird_im_diff.csv", im_diff, delimiter=",")
	# calculate stats
	max_diff = np.max(im_diff)
	mean = np.average(im_diff)
	std = np.std(im_diff)
	thresh = mean + 2 * std
	retval, im_thresh = cv2.threshold(im_diff.astype(np.uint8), thresh, 255, cv2.THRESH_BINARY)
	plt.gray()
	plt.imshow(im_thresh)
	plt.show()
	# opening followed by closing
	kernel = np.ones((10, 10), np.uint8)
	after_open = cv2.morphologyEx(im_thresh, cv2.MORPH_OPEN, kernel)
	im_open_first = cv2.morphologyEx(after_open, cv2.MORPH_CLOSE, kernel)
	plt.imshow(after_open)
	plt.show()
	#
	connectivity = 8
	num_labels, labels, stats, centroids = \
		cv2.connectedComponentsWithStats(after_open, connectivity, cv2.CV_32S)
	# print('Number of connected components:', num_labels - 1)  # the background is one of the "labels"
	# print('\nHere are the stats:')
	# print('Upper left and lower right pixel, plus area')
	# print(stats)
	# print('\nHere are the centroids')
	# print(centroids)
	
	
	#print(num_labels)
	#print(labels)
	
	#print(stats)
	count = 0
	for row in stats:
		if row[4]>40:
			count += 1
		else:
			labels[row[0]:row[2], row[1]:row[3]] = 0
	if count < 17:
		print("NO")
	else:
		print("YES")
	
	#
	colors = np.random.randint(255, size=(num_labels, 3))
	colors[0] = np.array((0, 0, 0))
	#
	im_colored = colors[labels]
	#
	# final = np.zeros_like(im_colored)
	# final = np.where(final==0, 100, final)
	# im_colored = np.where(im_colored==0, 100, final)
	# print(final)
	
	#np.savetxt("bird_im_colored.csv", im_colored, delimiter=",")
	# print(im_colored.shape, im_colored.dtype)
	cv2.imwrite(img_out_name,im_colored)
	plt.imshow(im_colored.astype(np.uint8))
	plt.show()
