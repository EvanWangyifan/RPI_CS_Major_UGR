import cv2
import numpy as np


img = 255 * np.ones(shape=[512, 512, 3], dtype=np.uint8)
im = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

im[::] = 100
im[50:450, 50:450] = 200
print(im)
cv2.imshow('img', im)
cv2.waitKey(0)

cv2.imwrite('syntheticSquare.png', im) 