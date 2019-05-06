#!/usr/bin/env python

import os, sys, argparse
import numpy as np

def bin_gen(args):
	if (args.f == "fp32"):
		value = np.float32(float(args.d))
		data = np.ones(args.n, np.float32) * value
	elif (args.f == "fp16"):
		value = np.float16( float(args.d))
		data = np.ones(args.n, np.float16) * value
	elif (args.f == "fx32"):
		value = np.int32(int(args.d))
		data = np.ones(args.n, np.int32) * value
	elif (args.f == "fx16"):
		value = np.int16(int(args.d))
		data = np.ones(args.n, np.int16) * value
	elif (args.f == "fx8"):
		value = np.int8(int(args.d))
		data = np.ones(args.n, np.int8) * value
	else:
		raise UserWarning("Unknown binary format: %s" % (args.f))

	data.tofile(args.o)
	filesize=os.path.getsize(args.o)
	print('Gererate File: {}, Num: {}, Size: {} byte'.format(args.o, args.n, filesize))

def init_param(args):
	parser = argparse.ArgumentParser(description="Generate binary file with specifyed format and data")
	parser.add_argument("-o", type=str, required=True, default="output.bin",
		help="ouput binary filename")
	parser.add_argument("-f", type=str, required=True, default="fp32",
		help="binary format: <fp32|fp16|fx32|fx16|fx8>")
	parser.add_argument("-d", type=str, required=True, default="1",
		help="binary data value")
	parser.add_argument("-n", type=int, required=True, default=1,
		help="element number")

	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bin_gen(args)
