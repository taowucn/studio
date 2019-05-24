#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

def bin_rotate(args):
	width = args.width
	height = args.height
	chan = args.chan
	if ((width == 0) or (height == 0) or (chan == 0)):
		raise UserWarning("Invalid shape argument, %d, %d, %d" % (width, height, chan))

	im_data = np.fromfile(args.i, np.uint8)
	im_data = im_data.reshape(chan, height, width)

	if (args.r == 0):
		im_rotated = im_data
	elif (args.r == 90):
		im_rotated = im_data.swapaxes(1, 2)
		im_rotated = im_rotated[:,:,::-1]
	elif (args.r == 180):
		im_rotated = im_data[:,::-1,::-1]
	elif (args.r == 270):
		im_rotated = im_data.swapaxes(1, 2)
		im_rotated = im_rotated[:,::-1,:]
	else:
		raise UserWarning("Invalid degree: %d, range [0|90|180|270]" % (args.r))

	im_rotated.tofile(args.o)

def init_param(args):
	parser = argparse.ArgumentParser(description="Rotate binary file with specific degree")
	parser.add_argument("-i", type=str, required=True, default="img.rgb",
		help="input image filename")
	parser.add_argument("-r", type=int, required=True,
		help="rotate degree in clockwise, only [0|90|180|270]")
	parser.add_argument("-o", type=str, required=True,
		help="out image filename")

	parser.add_argument("-width", type=int, required=True,
		help="image width")
	parser.add_argument("-height", type=int, required=True,
		help="image height")
	parser.add_argument("-chan", type=int, required=True,
		help="image channel")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bin_rotate(args)
