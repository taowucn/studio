
DIR := /home/taowu/data/code/github/cv/caffe/caffe/build

all:
	g++ -o test.elf get_param_from_proto.cpp ${DIR}/src/caffe/proto/caffe.pb.cc \
	-I ${DIR}/src/caffe/proto/ -lprotobuf

clean:
	rm test.elf
