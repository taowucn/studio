#!/bin/bash


testCount=$1
num=0
CMD="echo ..."
if [ "$#" -ne 1 ]; then
	echo "please input testCount"
	echo "$0 [testCount]"
else
	until [ $num -gt $testCount ]
	do
		echo -e "Test ${num}"
		$CMD
		num=$((num+1));
	done
fi

