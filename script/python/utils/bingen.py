#!/usr/bin/env python

import os, sys, argparse
import numpy as np

dtype_str = "<float64|float32|float16|uint64|int64|uint32|int32|uint16|int16|uint8|int8>"

def bin_gen(args):
	if (args.r):
		data = np.arange(args.n, dtype=args.f)
	else:
		value = float(args.d)
		data = np.ones(args.n, dtype=args.f) * value

	data = data.astype(dtype=args.f)
	data.tofile(args.o)
	filesize=os.path.getsize(args.o)

	if (args.r):
		print('Generate File: {}, Data: {}, Num: {}, Size: {} byte'.format(args.o, "0~N", args.n, filesize))
	else:
		print('Generate File: {}, Data: {}, Num: {}, Size: {} byte'.format(args.o, value, args.n, filesize))

def init_param(args):
	parser = argparse.ArgumentParser(description="Generate binary file with specific format and data")
	parser.add_argument("-o", type=str, required=True, default="output.bin",
		help="ouput binary filename")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="binary format: " + dtype_str)
	parser.add_argument("-d", type=str, required=False, default="1",
		help="binary data value")
	parser.add_argument("-r", action="store_true", default=False,
		help="binary data value from 0 to N increase with 1")
	parser.add_argument("-n", type=int, required=True, default=1,
		help="element number")

	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bin_gen(args)
