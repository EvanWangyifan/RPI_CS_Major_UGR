import cv2
import numpy as np
import sys


# Using ransac to find FOEs
def RANSAC(img1_flow, img2_flow, iteration, inlier_dist):
	vect = img2_flow - img1_flow
	inlier_max = 0
	for i in range(iteration):
		# randomly generate sample index
		index1 = np.random.randint(0, vect.shape[0])
		index2 = np.random.randint(0, vect.shape[0])
		if index1 == index2:
			index2 = np.random.randint(0, vect.shape[0])
		# find possible FOE by calculating intersections
		start1 = img1_flow[index1]
		end1 = img2_flow[index1]
		start2 = img1_flow[index2]
		end2 = img2_flow[index2]
		intersect = np.ones((2, 2))
		intersect[:, 0] = end1 - start1
		intersect[:, 1] = end2 - start2
		intersect = np.dot(np.linalg.inv(intersect), (start2 - start1))
		possibleFOE = start1 + intersect[0] * (end1 - start1)
		# find inliers & outliers
		inlier_index = dict()
		for j in range(vect.shape[0]):
			a = vect[j][1]
			b = -1 * vect[j][0]
			c = img2_flow[j][0]*img1_flow[j][1]-img2_flow[j][1]*img1_flow[j][0]
			dist = abs(a * possibleFOE[0] + b * possibleFOE[1] + c) / np.sqrt(a ** 2 + b ** 2)
			if dist < inlier_dist:
				inlier_index[j] = dist
				
		if len(inlier_index) > inlier_max:
			inlier_max = len(inlier_index)
			optimal_FOE = [possibleFOE[0], possibleFOE[1]]
			l_inliers = []
			l_outliers = []
			outlier_index = []
			diff = list(inlier_index.keys())
			for k in range(vect.shape[0]):
				if k not in diff:
					outlier_index.append(k)
			for arg in diff:
				l_inliers.append((list(img1_flow[arg]), list(img2_flow[arg])))
			for arg in outlier_index:
				l_outliers.append((list(img1_flow[arg]), list(img2_flow[arg])))
				
	print("********** Focus of expansion (FOE) **********")
	print(optimal_FOE)
	print()
	print("With inlier requirement of distance within {} pixels".format(inlier_dist))
	print("********** Inliers ({} in total) **********".format(inlier_max))
	for item in l_inliers:
		print("start: {}\tend:{}".format(item[0],item[1]))
	print()
	
	return l_inliers, optimal_FOE
	

# USAGE: python hw7.py 000005
if __name__ == "__main__":
	img1 = cv2.imread('hw7data\\' + str(sys.argv[1]) + '_10.png')
	img2 = cv2.imread('hw7data\\' + str(sys.argv[1]) + '_11.png')
	
	img1_clone = img1.copy()
	img2_clone = img2.copy()
	img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
	img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
	
	print("\nfeature extraction started :)")
	features1 = cv2.goodFeaturesToTrack(img1, mask=None,
	                                    **dict(maxCorners=200, qualityLevel=0.6, minDistance=6, blockSize=3))
	features2, status, err = cv2.calcOpticalFlowPyrLK(img1, img2, features1, None, **dict(winSize=(8, 8), maxLevel=5,
	                                                                                      criteria=(
	                                                                                      cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT,
	                                                                                      10, 0.03)))
	
	# Selection of points to estimate motion & Estimation of apparent motion
	# total features
	
	print("# Keypoints found: {}".format(features1.shape[0]))
	# features where status==1 (in motion)
	flow1 = features1[status == 1]
	flow2 = features2[status == 1]
	print("# Keypoints that are in motion: {}".format(flow1.shape[0]))
	# thresholding
	norms = np.linalg.norm(flow2 - flow1, axis=1)
	img1_flow = flow1[np.where(norms > 2)]
	img2_flow = flow2[np.where(norms > 2)]
	print("# Keypoints after thresholding: {}".format(img1_flow.shape[0]))
	print()
	
	# RANSAC
	iteration = 50
	inlier_dist = 2  # pixel distance allowed to be inlier
	l_inliers, optimal_FOE = RANSAC(img1_flow, img2_flow, iteration, inlier_dist)
	
	# draw FOE point
	out = img2_clone
	if len(l_inliers) > 3:
		print("Camera motion detected !!")
		center_coordinates = (int(optimal_FOE[0]), int(optimal_FOE[1]))
		radius = 20
		FOE_color = (0,165,255)  # orange
		FOE_thickness = -1
		out = cv2.circle(out, center_coordinates, radius, FOE_color, FOE_thickness)
		print("FOE has been drawn !!!")
	else:
		print("No camera motion since inlier count = {}".format(len(l_inliers)))
	
	# draw arrows
	arrow_color = (0, 255, 0)  # green
	arrow_thickness = 5
	for i in range(len(l_inliers)):
		start_point = (int(l_inliers[i][0][0]), int(l_inliers[i][0][1]))
		end_point = (int(l_inliers[i][1][0]), int(l_inliers[i][1][1]))
		out = cv2.arrowedLine(out, start_point, end_point, arrow_color, arrow_thickness)
		
	print("Arrowed lines have been drawn !!!")
	cv2.imwrite(str(sys.argv[1])+'_output1.png', out)
	print("<{}> has been outputed !!!!\n".format(str(sys.argv[1])+'_output1.png'))
	
	exit(0)