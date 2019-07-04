#!/usr/bin/env python

import os, sys, argparse
import numpy as np

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def bin_conv(args):
	data = np.fromfile(args.i, dtype=args.f).astype(args.t)
	data.tofile(args.o)
	print('Convert File: {} -> {}, Format: {} -> {}'.format(args.i, args.o, args.f, args.t))

def init_param(args):
	parser = argparse.ArgumentParser(description="Convert binary file from specific format to another format")
	parser.add_argument("-i", type=str, required=True, default="input.bin",
		help="input image filename")
	parser.add_argument("-o", type=str, required=True, default="output.bin",
		help="ouput binary filename")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="src binary format: " + dtype_str)
	parser.add_argument("-t", type=str, required=True, default="fp32",
		help="dest binary format: " + dtype_str)

	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bin_conv(args)
