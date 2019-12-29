#!/usr/bin/env python

import os, sys, argparse
import numpy as np

def bbox_trans(args):
	a = np.fromfile(args.i, np.float32)
	a = a.reshape(100, 4)
	b = np.zeros((100, 4),np.float32)
	b[:, :] = a[:, [1, 0, 3, 2]]
	#b [ : , : ] = a[ : , [1,0,3,2]]
	b.tofile(args.o)

def init_param(args):
	parser = argparse.ArgumentParser(description="Tranform bbox from format (y1, x1, y2, x2) to (x1, y1, x2, y2)")
	parser.add_argument("-i", type=str, required=True,
		help="input bbox filename")
	parser.add_argument("-o", type=str, required=True,
		help="out bbox filename")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	bbox_trans(args)
