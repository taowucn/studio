#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

def im_rotate_bin(args):
	im_data = np.fromfile(args.i, np.uint8)
	width = args.width
	height = args.height
	chan = args.chan
	if ((width == 0) or (height == 0) or (chan == 0)):
		raise UserWarning("Invalid argument, %d, %d, %d" % (width, height, chan))

	im_data = im_data.reshape(chan, height, width)

	# rgb -> bgr;  (c, w, h) -> (h, w, c)
	im_data = im_data[::-1,:,:]
	im = im_data.transpose(1,2,0)
	#for i in range(3):
	#	im[:,:,i] = im_data[2-i,:,:]

	center = (width/2, height/2)
	matix = cv2.getRotationMatrix2D(center, args.r, 1.0)
	im_rotated = cv2.warpAffine(im, matix, (width, height))

	for i in range(3):
		im_data[i,:,:] = im_rotated[:,:,2-i]
	#im_data = im_rotated

	im_data.tofile(args.o)

def im_rotate_img(args):
	im = cv2.imread(args.i)
	height = im.shape[0]
	width = im.shape[1]
	center = (width/2, height/2)

	matix = cv2.getRotationMatrix2D(center, args.r, 1.0)
	im_rotated = cv2.warpAffine(im, matix, (width, height))

	cv2.imwrite(args.o, im_rotated)

def im_rotate(args):
	if (args.b):
		im_rotate_bin(args)
	else:
		im_rotate_img(args)

def init_param(args):
	parser = argparse.ArgumentParser(description="Rotate image file with specific degree")
	parser.add_argument("-i", type=str, required=True, default="img.jpg",
		help="input image filename")
	parser.add_argument("-r", type=int, required=True,
		help="rotate degree")
	parser.add_argument("-o", type=str, required=True,
		help="out image filename")

	## For binary rotate
	parser.add_argument("-b", action='store_true',
		help="input is image or binary format, default is image")
	parser.add_argument("-width", type=int, required=False,
		help="image width")
	parser.add_argument("-height", type=int, required=False,
		help="image height")
	parser.add_argument("-chan", type=int, required=False,
		help="image channel number, range [1|3]")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	im_rotate(args)
