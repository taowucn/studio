#!/usr/bin/python2

import numpy as np
import sys
import os
import cv2

count = len(sys.argv)
if (count != 2):
	print "Usage: single_chan.py filename"
	exit()

file_in = os.path.basename(sys.argv[1])
#file_in = "out_sobel.bin"
#file_in = "rgb640x480.bin"

c = 1
h = 480
w = 640

data = np.fromfile(file_in, np.uint8)
data = data.reshape(c, h, w)

im = np.zeros((h, w, c), np.uint8)
im[:,:,0] = data[0,:,:]

cv2.namedWindow(file_in, 0)
cv2.imshow(file_in, im)
cv2.waitKey(0)
cv2.destroyAllWindows()
