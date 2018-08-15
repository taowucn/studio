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

c = 3
h = int(720)
w = (int(1280) + 31) >> 5 << 5

im_data = np.fromfile(file_in, np.uint8)
im_data = im_data.reshape(c, h, w)

im = np.zeros((h, w, c), np.uint8)
im[:,:,0] = im_data[2,:,:]
im[:,:,1] = im_data[1,:,:]
im[:,:,2] = im_data[0,:,:]

#res = cv2.resize(im, (1920, 1080))
cv2.namedWindow(file_in, 0)
cv2.imshow(file_in, im)
# cv2.imshow("s", im[:,:,0])
cv2.waitKey(0)
cv2.destroyAllWindows()

#rs = np.zeros((c, 1080, 1920), np.uint8)
#rs[2,:,:] = res[:,:,0]
#rs[1,:,:] = res[:,:,1]
#rs[0,:,:] = res[:,:,2]
#rs.tofile("pic1080.bin")
