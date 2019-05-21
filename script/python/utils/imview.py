#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

def im_view(args):
	im_data = np.fromfile(args.i, np.uint8)

	if (args.t):
		im_data = im_data.reshape(args.height, args.width, args.chan)
	else:
		im_data = im_data.reshape(args.chan, args.height, args.width)

	print(im_data.shape)
	im = np.zeros((args.height, args.width, args.chan), np.uint8)

	if (args.chan == 3):
		if (args.t):
			im = im_data
		else:
			im[:,:,0] = im_data[2,:,:]
			im[:,:,1] = im_data[1,:,:]
			im[:,:,2] = im_data[0,:,:]
	elif (args.chan == 1):
		im[:,:,0] = im_data[0,:,:]
	else:
		raise UserWarning("Unsupport channel: %d, range [1|3]" % (args.chan))

	if (args.o):
		cv2.imwrite(args.o, im)

	cv2.namedWindow(args.i, 0)
	cv2.imshow(args.i, im)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def init_param(args):
	parser = argparse.ArgumentParser(description="View image file with specific resolution/channel")
	parser.add_argument("-i", type=str, required=True, default="img.bin",
		help="input image filename, only fx8 format")
	parser.add_argument("-width", type=int, required=True,
		help="image width")
	parser.add_argument("-height", type=int, required=True,
		help="image height")
	parser.add_argument("-chan", type=int, required=True,
		help="image channel number, range [1|3]")
	parser.add_argument("-o", type=str, required=False,
		help="out image filename")
	parser.add_argument("-t", action='store_true',
		help="RGB is interleave, default is on channel ")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	im_view(args)
