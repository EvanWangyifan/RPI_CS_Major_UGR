import cv2
import numpy as np
import sys
import glob
import os


# STEP 2 & 3: find matching img pair
# Some codes adapted from: https://docs.opencv.org/4.x/dc/dc3/tutorial_py_matcher.html
def img_matching(img1, img2, fname1, fname2, dict_f_kp_des, out_dir):
	img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
	img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
	kp1, des1 = dict_f_kp_des[fname1]
	kp2, des2 = dict_f_kp_des[fname2]
	fname1 = fname1.split('\\')[1].split('.')[0]
	fname2 = fname2.split('\\')[1].split('.')[0]
	print("\n----------STEP2: matching for {} and {} STARTS----------".format(fname1, fname2))
	# BFMatcher with default params
	bf = cv2.BFMatcher()
	matches = bf.knnMatch(des1, des2, k=2)
	# Apply ratio test
	good = []
	img1_matching_points = []
	img2_matching_points = []
	for m, n in matches:
		if m.distance < 0.8 * n.distance:
			good.append([m])
			img1_matching_points.append(kp1[m.queryIdx].pt)
			img2_matching_points.append(kp2[m.trainIdx].pt)
	print("# of Matching keypoints: {}".format(len(good)))
	print("RATIO of matching to total in {}: {:.3f}".format(fname1, len(good) / len(kp1)))
	print("RATIO of matching to total in {}: {:.3f}".format(fname2, len(good) / len(kp2)))
	# check if percentage of match too small
	step3_flag = True
	if len(good) / len(kp1) < 0.05 or len(good) / len(kp2) < 0.05:
		step3_flag = False
	img_matching_lines = cv2.drawMatchesKnn(img1, kp1, img2, kp2, good, None, flags=2)
	# cv2.imshow("a", img3)
	# cv2.waitKey(0)
	path = os.path.join(out_dir, fname1 + '_' + fname2 + '_STEP2.jpg')
	print("STEP2: drawMatches graph {} created".format(path))
	cv2.imwrite(path, img_matching_lines)
	print("----------STEP2: matching for {} and {} ENDS----------\n".format(fname1, fname2))
	
	return good, img1_matching_points, img2_matching_points, step3_flag


# STEP 4: generate F
# part of the code adapted from: https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html
def generate_F(good, img1, img2, f_name1, f_name2, img1_matching_points, img2_matching_points, out_dir):
	img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
	img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
	sift = cv2.SIFT_create()
	kp1, des1 = sift.detectAndCompute(img1, None)
	kp2, des2 = sift.detectAndCompute(img2, None)
	F, Mask = cv2.findFundamentalMat(np.array(img1_matching_points), np.array(img2_matching_points), cv2.FM_RANSAC, 1.0, 0.99)
	# We select only inlier points
	inlier = good[Mask.ravel() == 1]
	print("\n----------STEP4----------".format())  # The number and percentage of matches that are inliers.
	print("# of matches that are F inlier: {}".format(inlier.shape[0]))
	print("% of matches that are F inlier: {:.2f}% ({}/{})".format(100 * inlier.shape[0] / len(good), inlier.shape[0], len(good)))
	img_F = cv2.drawMatchesKnn(img1, kp1, img2, kp2, inlier, None, flags=2)
	path = os.path.join(out_dir, f_name1 + '_' + f_name2 + '_STEP4.jpg')
	print("STEP4: drawMatches_F graph {} created".format(path))
	cv2.imwrite(path, img_F)
	# STEP 5:
	step5_flag = True
	if inlier.shape[0] < 0.2 * good.shape[0]:
		step5_flag = False
	return step5_flag


# STEP 6 & 7
def generate_H(good, img1, img2, f_name1, f_name2, img1_matching_points, img2_matching_points, out_dir):
	img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
	img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
	sift = cv2.SIFT_create()
	kp1, des1 = sift.detectAndCompute(img1, None)
	kp2, des2 = sift.detectAndCompute(img2, None)
	H, Stat = cv2.findHomography(img1_matching_points, img2_matching_points, cv2.RANSAC, 1.0, 0.99)
	# We select only inlier points
	inlier = good[Stat.ravel() == 1]
	print("\n----------STEP6----------".format())  # The number and percentage of matches that are inliers.
	print("# of matches that are H inlier: {}".format(inlier.shape[0]))
	print("% of matches that are H inlier: {:.2f}% ({}/{})".format(100 * inlier.shape[0] / len(good), inlier.shape[0], len(good)))
	img_H = cv2.drawMatchesKnn(img1, kp1, img2, kp2, inlier, None, flags=2)
	path = os.path.join(out_dir, f_name1 + '_' + f_name2 + '_STEP6.jpg')
	print("STEP6: drawMatches_H graph {} created".format(path))
	cv2.imwrite(path, img_H)
	# STEP 7:
	step7_flag = True
	if inlier.shape[0] < 0.4 * good.shape[0]:
		step7_flag = False
	return step7_flag


#  python hw4_align.py office out
if __name__ == '__main__':
	if len(sys.argv) != 3:
		print("USAGE: python hw4_align.py in_dir out_dir")
		exit(1)
	
	in_dir = sys.argv[1]
	out_dir = sys.argv[2]
	if not os.path.exists(out_dir):
		os.makedirs(out_dir)
	print("\nout_dir <{}> has been created :)\n".format(out_dir))
	l_dir_files = []  # store file with dir and type
	l_fname = []  # store only file name
	for f in glob.glob('{}/*.jpg'.format(in_dir)):
		l_dir_files.append(f)
		l_fname.append(f.split('\\')[1].split('.')[0])
	
	# go through every combination of files in the dir
	l_file_pairs = []  # list of all img file combinations
	for i in range(len(l_dir_files)):
		for j in range(i + 1, len(l_dir_files)):
			l_file_pairs.append((l_dir_files[i], l_dir_files[j]))
	
	# STEP 1: Extract the keypoints and descriptors in each image.
	dict_f_kp_des = dict()  # keypoints for each file
	for i in range(len(l_dir_files)):
		img = cv2.imread(l_dir_files[i])
		img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		sift = cv2.SIFT_create()
		kp, des = sift.detectAndCompute(img, None)
		dict_f_kp_des[l_dir_files[i]] = (kp, des)
		print("STEP1: # of keypoints in {} is: {}".format(l_fname[i], len(kp)))
	
	for f1, f2 in l_file_pairs:
		img1 = cv2.imread(f1)
		img2 = cv2.imread(f2)
		f_name1 = f1.split('\\')[1].split('.')[0]
		f_name2 = f2.split('\\')[1].split('.')[0]
		# STEP 2 & 3
		good, img1_matching_points, img2_matching_points, step3_flag = img_matching(img1, img2, f1, f2, dict_f_kp_des, out_dir)
		print('\n----------STEP3: match percentage thresholding')
		if step3_flag:
			print("----------STEP3: GOOD enough, continue")
		else:
			print("----------STEP3: Too LOW percentage of match----------")
			print("STEP3: stop attempting to match {} and {}".format(f1, f2))
			continue
		# STEP 4 & 5
		step5_flag = generate_F(np.array(good), img1, img2, f_name1, f_name2, np.array(img1_matching_points), np.array(img2_matching_points), out_dir)
		if step5_flag:
			print("\n----------STEP5: GOOD enough, continue")
		else:
			print("\n----------STEP5: Too LOW percentage of match----------")
			print("STEP5: stop attempting to match {} and {}".format(f1, f2))
			continue
		# STEP 6 & 7
		step7_flag = generate_H(np.array(good), img1, img2, f_name1, f_name2, np.array(img1_matching_points), np.array(img2_matching_points), out_dir)
		if step7_flag:
			print("\n----------STEP7: GOOD enough, continue")
		else:
			print("\n----------STEP7: Too LOW percentage of match----------")
			print("STEP7: stop attempting to match {} and {}\n\n".format(f1, f2))
			continue
		# STEP 8: MOSAIC
		
		
		