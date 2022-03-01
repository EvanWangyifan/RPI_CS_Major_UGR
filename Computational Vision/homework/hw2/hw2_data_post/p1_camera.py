import sys
import numpy as np


def generate_M(Rx, Ry, Rz, tx, ty, tz, f, d, ic, jc):
    # rotate and translate
    rotated_matrix = np.dot(np.dot(Rx, Ry), Rz)  # R
    t = np.array([tx, ty, tz])
    t_prime = -np.dot(np.transpose(rotated_matrix), t)  # t' = -Rt
    t_prime = np.array([t_prime])
    rotated_translated_matrix = np.concatenate((np.transpose(rotated_matrix), np.array(t_prime.T)), axis=1)
    # find intrinsic matrix K
    K = [[f / d, 0, jc], [0, f / d, ic], [0, 0, 1]]
    K = np.vstack(K)
    # Multiply rotated translated matrix by the intrinsic matrix
    return np.dot(K, rotated_translated_matrix)


def matrix_printer(m):
    for line in m:
        print("{:.1f}, {:.1f}, {:.1f}, {:.1f}".format(line[0], line[1], line[2], line[3]))
    return


# command line: python p1_camera.py params_p1_v1.txt points_p1_v1.txt
#               python p1_camera.py params_p1_v2.txt points_p1_v2.txt
if __name__ == "__main__":
    f = open(sys.argv[1])
    params = []
    for line in f:
        params.append(line.strip().split())
    # read in all params
    rx = np.deg2rad(float(params[0][0]))
    ry = np.deg2rad(float(params[0][1]))
    rz = np.deg2rad(float(params[0][2]))
    tx = float(params[1][0])
    ty = float(params[1][1])
    tz = float(params[1][2])
    f = float(params[2][0])
    d = float(params[2][1]) / 1000  # mm to micron
    ic = int(params[2][2])
    jc = int(params[2][3])
    # explicitly form the three rotations matrices and compose them.
    Rx = [[1, 0, 0], [0, np.cos(rx), -np.sin(rx)], [0, np.sin(rx), np.cos(rx)]]
    Rx = np.vstack(Rx)
    Ry = [[np.cos(ry), 0, np.sin(ry)], [0, 1, 0], [-np.sin(ry), 0, np.cos(ry)]]
    Ry = np.vstack(Ry)
    Rz = [[np.cos(rz), - np.sin(rz), 0], [np.sin(rz), np.cos(rz), 0], [0, 0, 1]]
    Rz = np.vstack(Rz)
    # part 1: output M matrix
    M = generate_M(Rx, Ry, Rz, tx, ty, tz, f, d, ic, jc)
    print("Matrix M:")
    matrix_printer(M)

    # part 2: project the point into image
    pts = np.loadtxt(sys.argv[2])
    ones = np.array([[1] * pts.shape[0]])
    pts_with_t = np.concatenate((pts, np.array(ones.T)), axis=1)
    pixel_location = np.dot(M, np.transpose(pts_with_t))
    # calculate the projected location at the image
    pixel_location[0] = pixel_location[0] / pixel_location[-1]
    pixel_location[1] = pixel_location[1] / pixel_location[-1]
    print("Projections:")
    str_visible = "visible:"
    str_hidden = "hidden:"
    for i in range(len(pixel_location[0])):
        proj_x = pixel_location[0][i]
        proj_y = pixel_location[1][i]
        # determine if inside boundary or not
        inside_boundary = 0 <= proj_y <= 4000 and 0 <= proj_x <= 6000
        inside_outside = "outside"
        if inside_boundary:
            inside_outside = "inside"
        print("{}: {:.1f} {:.1f} {:.1f} => {:.1f} {:.1f} {}".format(i, pts[i][0], pts[i][1], pts[i][2], proj_y, proj_x,
                                                                    inside_outside))
        # find if visible or not
        rotated_matrix = np.dot(np.dot(Rx, Ry), Rz)
        # camera coord
        p_prime = np.dot(rotated_matrix, np.array([0, 0, 0])) + np.array([tx, ty, tz])
        camera_axis = np.dot(rotated_matrix, np.array([0, 0, 1]))
        camera_axis /= np.linalg.norm(camera_axis)
        visible = np.dot(camera_axis, pts[i] - p_prime)
        if visible > 0:
            str_visible += ' ' + str(i)
        else:
            str_hidden += ' ' + str(i)

    # print formatted visible & hidden
    print(str_visible)
    if len(str_hidden) > 7:
        print(str_hidden)
