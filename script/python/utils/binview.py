#!/usr/bin/env python

import numpy as np
import sys
import os

argc = len(sys.argv)
if (argc < 2):
    print "Usage:  binview.py <input_filename> <fp32|fp16|fx32|fx16|fx8> <Row>\n"
    exit()

in_file = sys.argv[1]
data_format = sys.argv[2]
width = 0
if (argc == 4):
	width = int(sys.argv[3])

if (data_format == "fp32"):
    data = np.fromfile(in_file, np.float32)
elif (data_format == "fp16"):
    data = np.fromfile(in_file, np.float16)
elif (data_format == "fx32"):
	data = np.fromfile(in_file, np.int32)
elif (data_format == "fx16"):
	data = np.fromfile(in_file, np.int16)
elif (data_format == "fx8"):
	data = np.fromfile(in_file, np.int8)
else:
    print("Invalid data format: " + data_format)
    exit()

if (width != 0):
	num = data.shape[0]
	height = num / width
	data = data.reshape(height, width)

print(data)

