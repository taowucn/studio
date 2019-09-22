#!/bin/sh

export CAFFE_ROOT=/home/taowu/data/vmdisk/code/github/cv/caffe/caffe
cp classify.py ${CAFFE_ROOT}/python

cd ${CAFFE_ROOT} && \
python3 python/classify.py --model_def examples/cifar10/cifar10_quick.prototxt --pretrained_model examples/cifar10/cifar10_quick_iter_5000.caffemodel --center_only  examples/images/cat.jpg --label data/cifar10/batches.meta.txt foo
