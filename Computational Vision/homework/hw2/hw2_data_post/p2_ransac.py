import numpy as np
import sys


def RANSAC(pts, a, b, c, tau):
    inlier_count = 0
    inlier_dist = 0
    outlier_dist = 0
    for p in pts:
        # formula got from: https://www.youtube.com/watch?v=BHNNz6jCuHw
        distance = abs(a * p[0] + b * p[1] + c) / np.sqrt(np.square(a) + np.square(b))
        if distance > tau:
            outlier_dist += distance
        else:
            inlier_dist += distance
            inlier_count += 1
    return [inlier_dist, outlier_dist, inlier_count]


# cmd: python p2_ransac.py p2_pts1_in.txt 25 2.5 999
if __name__ == '__main__':
    f_pts = open(sys.argv[1])
    pts = []
    for line in f_pts:
        line = line.strip().split()
        pts.append(np.array([float(line[0]), float(line[1])]))
    pts = np.array(pts)
    samples = int(sys.argv[2])
    tau = float(sys.argv[3])
    if len(sys.argv) > 4:
        seed = int(sys.argv[4])
    np.random.seed(seed)
    max_inlier_count = 0
    final_statistics = ""
    for i in range(samples):
        indices = np.random.randint(0, len(pts), 2)  # generated (x,y)
        if indices[0] == indices[1]:  # the two indices are equal
            continue
        # form a line
        x1 = pts[indices[0]][0]
        y1 = pts[indices[0]][1]
        x2 = pts[indices[1]][0]
        y2 = pts[indices[1]][1]
        # formula Y = mX + c
        m = (y2 - y1) / (x2 - x1)
        c = y1 - m * x1
        # convert y=mx+c to ax+by+c=0
        a = -1 * m
        b = 1
        c = -1 * c
        # call RANSAC
        RANSAC_result = RANSAC(pts, a, b, c, tau)
        inlier_dist = RANSAC_result[0]
        outlier_dist = RANSAC_result[1]
        inlier_count = RANSAC_result[2]
        if inlier_count > max_inlier_count:
            print("Sample {}:".format(i))
            print("indices ({},{})".format(indices[0], indices[1]))
            scale = 1 / np.sqrt(a ** 2 + b ** 2)
            if c > 0:
                a *= -1
                b *= -1
                c *= -1
            print("line ({:.3f},{:.3f},{:.3f})".format(a * scale, b * scale, c * scale))
            print("inliers {}\n".format(inlier_count))
            final_statistics = ""
            final_statistics += "avg inlier dist {:.3f}\navg outlier dist {:.3f}".format(inlier_dist / inlier_count,
                                                                            outlier_dist / (len(pts) - inlier_count))
            max_inlier_count = inlier_count
    print(final_statistics)
