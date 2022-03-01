import cv2
import numpy as np
import sys

pi = np.pi


def cvt_dir_clr(im_dir):
	im_dir.astype(np.int32)
	result = np.zeros((im_dir.shape[0], im_dir.shape[1], 3)).astype(np.int32)
	clr_map = np.array([(0, 0, 255), (0, 255, 0), (255, 0, 0), (255, 255, 255), (0, 0, 0)])
	result = clr_map[im_dir].astype(np.int32)
	return result


def non_maximum_suppression(im_dir, im_gm):
	# store max point as 1 and nonmax as -99
	result = np.zeros_like(im_dir).astype(np.int32)
	result = np.where(result == 0, -999, result)
	# check every pixel point
	for i in range(im_dir.shape[0]):
		for j in range(im_dir.shape[1]):
			# make comparison with direction neighbor, fill the number
			if im_dir[i][j] != -1:
				result[i][j] = neighbor_compare(im_dir[i][j], im_gm, i, j)
	return result


def neighbor_compare(dir, img_gm, i, j):
	result = -999
	behind = -np.inf
	ahead = -np.inf
	if dir == 0:  # W/E check (i, j-1) and (i, j+1)
		if j - 1 >= 0:
			behind = img_gm[i][j - 1]
		if j + 1 < img_gm.shape[1]:
			ahead = img_gm[i][j + 1]
		if img_gm[i][j] >= behind and img_gm[i][j] >= ahead:
			result = 1
	elif dir == 1:  # NW/SE check (i-1,j-1) and (i+1,j+1)
		if i - 1 >= 0 and j - 1 >= 0:
			behind = img_gm[i - 1][j - 1]
		if i + 1 < img_gm.shape[0] and j + 1 < img_gm.shape[1]:
			ahead = img_gm[i + 1][j + 1]
		if img_gm[i][j] >= behind and img_gm[i][j] >= ahead:
			result = 1
	elif dir == 2:  # N/S check (i-1, j) and (i+1, j)
		if i - 1 >= 0:
			behind = img_gm[i - 1][j]
		if i + 1 < img_gm.shape[0]:
			ahead = img_gm[i + 1][j]
		if img_gm[i][j] >= behind and img_gm[i][j] >= ahead:
			result = 1
	elif dir == 3:  # NE/SW check (i-1,j+1) and (i+1,j-1)
		if i - 1 >= 0 and j + 1 < img_gm.shape[1]:
			behind = img_gm[i - 1][j + 1]
		if i + 1 < img_gm.shape[0] and j - 1 >= 0:
			ahead = img_gm[i + 1][j - 1]
		if img_gm[i][j] >= behind and img_gm[i][j] >= ahead:
			result = 1
	return result


if __name__ == '__main__':
	if len(sys.argv) > 3:
		print("USAGE: python p2_edge.py sigma in_img")
		exit(1)
	# take gradient magnitude
	sigma = float(sys.argv[1])
	img = sys.argv[2]
	prefix = img.split('.')[0]
	filetype = img.split('.')[1]
	img = cv2.imread(img, cv2.IMREAD_GRAYSCALE).astype(np.float32)
	ksize = (int(4 * sigma + 1), int(4 * sigma + 1))
	im_s = cv2.GaussianBlur(img.astype(float), ksize, sigma)
	kx, ky = cv2.getDerivKernels(1, 1, 3)
	kx = np.transpose(kx / 2)
	ky = ky / 2
	im_dx = cv2.filter2D(im_s, -1, kx)
	im_dy = cv2.filter2D(im_s, -1, ky)
	im_gm = np.sqrt(im_dx ** 2 + im_dy ** 2)  # gradient magnitude
	# gradient angle
	im_angle = np.arctan2(im_dy, im_dx)
	normalized_mag = im_gm / np.max(im_gm) * 255
	# output grd file
	cv2.imwrite('{}_grd.{}'.format(prefix, filetype), normalized_mag)
	# calculate the angle
	im_angle = np.where(im_angle < 0, im_angle + pi, im_angle)
	im_angle = np.fmod(im_angle + (pi / 8), pi)
	# use the angle to know the direction: (-1,black)(0,W/E,red)(1,NW/SE,green)(2,N/S,blue)(3,NE/SW,white)
	im_dir = np.zeros_like(im_angle).astype(np.int32)
	im_dir = im_angle // (pi / 4)  # map to 0-3
	im_dir = np.where(im_gm == 0, -1, im_dir)
	# mark all boarder -1 black
	im_dir[0, :] = -1
	im_dir[-1, :] = -1
	im_dir[:, 0] = -1
	im_dir[:, -1] = -1
	# map direction int to output colors
	out_dir = im_dir.copy()
	out_dir = np.where(im_gm < 1, -1, out_dir)
	out_dir = cvt_dir_clr(out_dir.astype(np.int32))
	cv2.imwrite('{}_dir.{}'.format(prefix, filetype), out_dir)
	# Do Non-Maximal_Suppression
	suppressed_img = non_maximum_suppression(im_dir, im_gm)
	print("Number after non-maximum: {}".format(len(np.where(suppressed_img == 1)[0])))
	# Do 1.0 threshold
	suppressed_img = np.where(im_gm < 1.0, -999, suppressed_img)
	print("Number after 1.0 threshold: {}".format(len(np.where(suppressed_img == 1)[0])))
	# get all stats
	suppressed_img = np.where(suppressed_img == 1, suppressed_img * im_gm, suppressed_img)
	pts_remaining = suppressed_img[suppressed_img != -999]
	avg = np.average(pts_remaining)
	std = np.std(pts_remaining)
	t = np.minimum(avg + std / 2, 30 / sigma)
	print("mu: {:.2f}".format(avg))
	print("s: {:.2f}".format(std))
	print("Threshold: {:.2f}".format(t))
	# generate final threshold img
	zeros = np.zeros_like(suppressed_img).astype(np.float32)
	im_thr = np.where(suppressed_img > t, suppressed_img, zeros)
	im_thr = im_thr / np.max(im_thr) * 255
	cv2.imwrite('{}_thr.{}'.format(prefix, filetype), im_thr)
	print("Number after threshold: {}".format(len(np.where(im_thr != 0)[0])))
