#!/bin/bash

input_file=$1
split_num=$2

usage()
{
	echo "Usage: $0 <Input File> <Split Num>"
	echo "           split one file into N * files"
}

## main
if [ $# -ne 2 ]; then
	usage
	exit 1
fi

total_size=$(ls -l ${input_file} |awk '{print $5}')
split_size=$((total_size/split_num))
is_multi=$((total_size%split_num))
echo "Total_size=${total_size}, N=${split_num}, Split_size=${split_size}"

if [ ${is_multi} -ne 0 ]; then
	echo "Input File=${input_file} can not be split to N=${split_num}"
	echo "${total_size} / ${split_num} != 0"
fi

dir_name=$(dirname ${input_file})
base_name=$(basename ${input_file})
skip_size=0
seq_no=0

echo "${dir_name}, ${base_name}"
while [ ${seq_no} -ne ${split_num} ];
do
	output_file=${dir_name}/${seq_no}_${base_name}
	skip_size=$((split_size*seq_no))

	dd if=${input_file} of=${output_file} count=${split_size} bs=1 skip=${skip_size}
	echo "[${seq_no}]: Split_file: ${output_file}, Skip_size:${skip_size}"
	
	seq_no=$((seq_no+1))
done
echo "Split Done"

