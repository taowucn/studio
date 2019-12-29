#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def im_view(args):
	im_data = np.fromfile(args.i, dtype=args.f)

	if (args.c == 1):
		im_data = im_data.reshape(args.height, args.width, args.chan)
	else:
		im_data = im_data.reshape(args.chan, args.height, args.width)

	print(im_data.shape)
	im = np.zeros((args.height, args.width, args.chan), dtype=args.f)

	if (args.chan == 3):
		if (args.c == 1):
			im = im_data
		elif (args.c == 2):
			im[:,:,0] = im_data[0,:,:]
			im[:,:,1] = im_data[1,:,:]
			im[:,:,2] = im_data[2,:,:]
		elif (args.c == 0):
			im[:,:,0] = im_data[2,:,:]
			im[:,:,1] = im_data[1,:,:]
			im[:,:,2] = im_data[0,:,:]
		else:
			raise UserWarning("Unsupport color format : %d, range [0,1,2]" % (args.c))

	elif (args.chan == 1):
		im[:,:,0] = im_data[0,:,:]
	else:
		raise UserWarning("Unsupport channel: %d, range [1|3]" % (args.chan))

	if (args.o):
		cv2.imwrite(args.o, im)
	else:
		if (args.f != "uint8" and args.f != "float32"):
			im = im.astype(np.uint8)
			print("Transform to uint8 for imshow");

		cv2.namedWindow(args.i, 0)
		cv2.imshow(args.i, im)
		cv2.waitKey(0)
		cv2.destroyAllWindows()

def init_param(args):
	parser = argparse.ArgumentParser(description="View image file with specific resolution/channel")
	parser.add_argument("-i", type=str, required=True, default="img.bin",
		help="input image filename, only fx8 format")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="input binary format: " + dtype_str)
	parser.add_argument("-width", type=int, required=True,
		help="image width")
	parser.add_argument("-height", type=int, required=True,
		help="image height")
	parser.add_argument("-chan", type=int, required=True,
		help="image channel number, range [1|3]")
	parser.add_argument("-o", type=str, required=False,
		help="out image filename, do not imshow in live if gen output image")
	parser.add_argument("-c", type=int, required=False, default=0,
		help="color format of the binary, 0: RGB on Channel (default); 1: RGB is interleave; 2: BGR.")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	im_view(args)
