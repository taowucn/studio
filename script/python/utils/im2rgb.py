#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

def im_to_rgb(args):
	im = cv2.imread(args.i)
	imshape = im.shape
	im_data = np.zeros((imshape[2], imshape[0], imshape[1]), np.uint8)
	#print(imshape)
	#print(im_data.shape)

	im_data[2,:,:] = im[:,:,0]
	im_data[1,:,:] = im[:,:,1]
	im_data[0,:,:] = im[:,:,2]

	if (args.o):
		im_data.tofile(args.o)

	cv2.namedWindow(args.i, 0)
	cv2.imshow(args.i, im)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def init_param(args):
	parser = argparse.ArgumentParser(description="Convert image(jpg/png/...) file to RGB")
	parser.add_argument("-i", type=str, required=True, default="img.jpg",
		help="input image filename, only fx8 format")
	parser.add_argument("-o", type=str, required=True, default="img.rgb",
		help="out image filename")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	im_to_rgb(args)
