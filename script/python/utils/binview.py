#!/usr/bin/env python

import os, sys, argparse
import numpy as np

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def bin_view(args):
	data = np.fromfile(args.i, dtype=args.f)

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
		help="input binary format: " + dtype_str)
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
