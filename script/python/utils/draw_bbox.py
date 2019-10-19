#!/usr/bin/env python

import os, sys, argparse
import numpy as np
import cv2

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def init_param(args):
	parser = argparse.ArgumentParser(description="Draw BBox on image by "
		"BBox binary in specific format")
	parser.add_argument("-i", type=str, required=True, default="input.jpg",
		help="input image filename")
	parser.add_argument("-b", type=str, required=True, default="bbox.bin",
		help="bbox binary filename, <x1, y1, x2, y2>")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="bbox binary format: " + dtype_str)
	parser.add_argument("-o", type=str, required=False, default="output.jpg",
		help="output image filename")
	parser.add_argument("--verbose", action="store_true", default=False,
		help="print verbose info")
	return parser.parse_args(args)

def draw_bbox(args):
	if (not os.path.exists(args.i)):
		raise UserWarning("Image %s not found." % (args.i))
	if (not os.path.exists(args.b)):
		raise UserWarning("BBox %s not found." % (args.b))

	## Parser bbox_format
	bbox_bin = np.fromfile(args.b, dtype=args.f)

	bbox_num = bbox_bin.shape[0]
	idx = 0
	valid_bbox_num = 0

	img = cv2.imread(args.i)
	font = cv2.FONT_HERSHEY_SIMPLEX
	color = (0, 255, 0) ## green
	line_bold = 1

	while idx < (bbox_num - 4):
		org_x1 = bbox_bin[idx]
		org_y1 = bbox_bin[idx + 1]
		org_x2 = bbox_bin[idx + 2]
		org_y2 = bbox_bin[idx + 3]
		idx += 4
		if (args.verbose):
			print(org_x1, org_y1, org_x2, org_y2)

		rect_x1 = int(org_x1)
		rect_y1 = int(org_y1)
		rect_x2 = int(org_x2)
		rect_y2 = int(org_y2)

		diff = rect_y2 - rect_y1 + rect_x2 - rect_x1
		if diff > 0:
			cv2.rectangle(img, (rect_x1, rect_y1), ((rect_x2, rect_y2)), color, line_bold)
			valid_bbox_num += 1

	print("BBox Num: %d/%d (valid/total)" % (valid_bbox_num, (bbox_num / 4)))
	cv2.imwrite(args.o, img)
	#cv2.imshow(out_img_file, img)
	#cv2.waitKey()
	#cv2.destroyAllWindows()

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	draw_bbox(args)
