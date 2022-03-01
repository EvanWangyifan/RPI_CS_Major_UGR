import cv2
import numpy as np
import sys


def generate_W(img_sobel):
	W = img_sobel
	W[:, 0] = np.inf
	W[:, -1] = np.inf
	for i in range(1, img_sobel.shape[0]):
		left = W[i - 1, :-2]
		right = W[i - 1, 2:]
		center = W[i - 1, 1:-1]
		W[i, 1:-1] += np.min((left, right, center), axis=0)
	return W


def generate_C(W):
	c = np.zeros(W.shape[0])
	c[-1] = np.argmin(W[-1, :])
	i = W.shape[0] - 2
	while i >= 0:
		lower_neighbors = [W[i][int(c[i + 1]) - 1], W[i][int(c[i + 1])], W[i][int(c[i + 1]) + 1]]
		c[i] = c[i + 1] + np.argmin(lower_neighbors) - 1
		i -= 1
	return c


if __name__ == '__main__':
	if len(sys.argv) != 2:
		print("USAGE: python p1_seam_carve.py img")
		exit(1)
	f_name = sys.argv[1]
	pic_name = f_name.split('.')[0]
	img = cv2.imread(f_name).astype(np.float32)
	row, col = img.shape[0], img.shape[1]
	# determine the direction of seaming and rotate if needed
	horizontal = False
	if row > col:
		horizontal = True
		img = np.transpose(img, (1, 0, 2))
	img_seam = img.copy()
	img_final = np.zeros_like(img)
	iterations = abs(row - col)
	# iterate for every seam
	for i in range(iterations):
		# use sobel method to create energy graph
		img_grey = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		img_sobel = abs(cv2.Sobel(img_grey, cv2.CV_64F, 1, 0)) + abs(cv2.Sobel(img_grey, cv2.CV_64F, 0, 1))
		# compute seam cost function W
		W = generate_W(img_sobel)
		# trace back through it to find the actual seam.
		C = generate_C(W).astype(np.int64)
		# create seam on the img
		if i == 0:
			for j in range(C.shape[0]):
				img_seam[j, C[j]] = [0, 0, 255]
		# crave the img_final
		img_final = np.zeros((W.shape[0], W.shape[1] - 1, 3)).astype(np.float32)
		for j in range(W.shape[0]):
			img_final[j] = np.vstack((img[j, :C[j]], img[j, C[j] + 1:]))
		# update img to the carved image
		img = img_final.copy()
		# printing outputs
		if i == 0 or i == 1 or i == iterations - 1:
			print("\nPoints on seam {}:".format(i))
			if horizontal:
				print("horizontal")
				print('{}, {}'.format(C[0], 0))
				print('{}, {}'.format(C[col // 2], col // 2))
				print('{}, {}'.format(C[col - 1], col - 1))
				print('Energy of seam {}: {:.2f}'.format(i, W[-1, C[-1]] / col))
			else:
				print("vertical")
				print('{}, {}'.format(0, C[0]))
				print('{}, {}'.format(row // 2, C[row // 2]))
				print('{}, {}'.format(row - 1, C[row - 1]))
				print('Energy of seam {}: {:.2f}'.format(i, W[-1, C[-1]] / row))
	# rotate back to og img for output
	if horizontal:
		img_seam = np.transpose(img_seam, (1, 0, 2))
		img_final = np.transpose(img_final, (1, 0, 2))
	# output seam_img file
	cv2.imwrite("{}_seam.jpg".format(pic_name), img_seam)
	cv2.imwrite("{}_final.jpg".format(pic_name), img_final)
	# cv2.imshow("og_img", img_copy.astype(np.uint8))
	# cv2.waitKey(0)
