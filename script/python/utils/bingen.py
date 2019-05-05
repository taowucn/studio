#!/usr/bin/env python

import numpy as np
import sys
import os

argc = len(sys.argv)
if (argc < 4):
        print "Usage: bingen.py filename value size\n"
        exit()

base = os.path.basename(sys.argv[1])
dirname = os.path.dirname(sys.argv[1])
val = int(sys.argv[2])

out_size = int(sys.argv[3])
out = np.ones(out_size, dtype=np.int8)*val
out_name = os.path.splitext(base)[0] + os.path.splitext(base)[1]
print out_name + '    ' + str(out.size) + ' byte'
out.tofile(os.path.join(dirname, out_name))

