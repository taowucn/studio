#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

def im_view(args):
	im_data = np.fromfile(args.i, np.uint8)
	im_data = im_data.reshape(args.chan, args.height, args.width)

	im = np.zeros((args.height, args.width, args.chan), np.uint8)
	im[:,:,0] = im_data[2,:,:]
	im[:,:,1] = im_data[1,:,:]
	im[:,:,2] = im_data[0,:,:]

	if (args.o):
		cv2.imwrite(args.o, im)

	cv2.namedWindow(args.i, 0)
	cv2.imshow(args.i, im)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def init_param(args):
	parser = argparse.ArgumentParser(description="View image file with specified resolution")
	parser.add_argument("-i", type=str, required=True, default="img.bin",
		help="input image filename")
	parser.add_argument("-width", type=int, required=True,
		help="image width")
	parser.add_argument("-height", type=int, required=True,
		help="image height")
	parser.add_argument("-chan", type=int, required=True,
		help="image channel number")
	parser.add_argument("-o", type=str, required=False, default="img.jpg",
		help="out image filename")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	im_view(args)
