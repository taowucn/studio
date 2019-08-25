#!/usr/bin/env python

import os, sys, argparse
import numpy as np

def transform(args):
	data = np.fromfile(args.i, dtype=args.f)

	## customize code: change reshape and transpose param
	dst = data.reshape(2, 2, 4).transpose(1, 0, 2)

	dst.tofile(args.o)
	print('Transpose File: {} -> {}'.format(args.i, args.o))

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def init_param(args):
	parser = argparse.ArgumentParser(description="Transpose binary file")
	parser.add_argument("-i", type=str, required=True, default="input.bin",
		help="input image filename")
	parser.add_argument("-o", type=str, required=True, default="output.bin",
		help="ouput binary filename")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="dest binary format: " + dtype_str)

	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	transform(args)

