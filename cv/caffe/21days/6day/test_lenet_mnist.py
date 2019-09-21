#!/usr/bin/env python

import caffe
import numpy as np
import os,sys
import argparse

CAFFE_ROOT="/home/taowu/data/vmdisk/code/github/cv/caffe/caffe"

def test_lenet_mnist(argv):
	caffe_model = CAFFE_ROOT + "/examples/mnist/lenet_iter_10000.caffemodel"
	labels_f = "labels.txt"
	deploy = "deploy.prototxt"

	img_f = args.i
	if (not os.path.exists(labels_f)):
		raise UserWarning("%s not found." % (labels_f))
	if (not os.path.exists(img_f)):
		raise UserWarning("%s not found." % (img_f))

	net = caffe.Net(deploy, caffe_model, caffe.TEST)
	trans = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
	trans.set_transpose('data', (2, 0, 1))
	trans.set_raw_scale('data', 255)

	im = caffe.io.load_image(img_f, False)
	net.blobs['data'].data[...] = trans.preprocess('data', im)
	out = net.forward()

	labels = np.loadtxt(labels_f, str, delimiter='\t')
	prob = net.blobs['Softmax1'].data[0].flatten()
	print("----- result ----")
	print(prob)
	order = prob.argsort()[-1]
	print(order)
	print('The result: %s' %labels[order])

def init_param(args):
	parser = argparse.ArgumentParser(description="Test Lenet MNIST")
	parser.add_argument("-i", type=str, required=True,
		help="Input image filename")
	return parser.parse_args(args)

if __name__ == '__main__':
	args = init_param(sys.argv[1:])
	test_lenet_mnist(args)
