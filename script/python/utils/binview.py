#!/usr/bin/env python

import os, sys, argparse
import numpy as np

def bin_view(args):
	if (args.f == "fp32"):
		data = np.fromfile(args.i, np.float32)
	elif (args.f == "fp16"):
		data = np.fromfile(args.i, np.float16)
	elif (args.f == "fx32"):
		data = np.fromfile(args.i, np.int32)
	elif (args.f == "fx16"):
		data = np.fromfile(args.i, np.int16)
	elif (args.f == "fx8"):
		data = np.fromfile(args.i, np.int8)
	else:
		raise UserWarning("Unknown binary format: %s" % (args.f))

	if (args.w > 0):
		num = data.shape[0]
		height = num / args.w
		data = data.reshape(height, args.w)

	if (args.q > 0):
		data_f = data.astype(np.float32)
		data_f = data_f/pow(2, args.q)
		if (args.v):
			for row in data_f:
				print(row)
		else:
			print(data_f)
	else:
		if (args.v):
			for row in data:
				print(row)
		else:
			print(data)

def init_param(args):
	parser = argparse.ArgumentParser(description="View binary file with specific format")
	parser.add_argument("-i", type=str, required=True, default="input.bin",
		help="input binary filename")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="input binary format: <fp32|fp16|fx32|fx16|fx8>")
	parser.add_argument("-q", type=int, required=False,
		help="Q value for quantized data")
	parser.add_argument("-w", type=int, required=False,
		help="Show element number in row (width)")
	parser.add_argument("-v", action='store_true', required=False,
		help="Show data by print txt")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bin_view(args)
