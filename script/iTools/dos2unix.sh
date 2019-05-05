#!/bin/bash

##########################
# History:
#	2011/08/15 - [Tao Wu] create file
#
# Copyright (C) 2004-2011, Ambarella, Inc.
##########################

function dos2unixd(){
	cd $1
	for i in `ls -1`
	do
		echo $i
		if [ -d $i ]; then
			dos2unixd $i
		elif [ -f $i ]; then
			if  [ "${i##*.}" == "cpp" ] || [ "${i##*.}" == "h" ] || [ "${i##*.}" == "mk" ] || [ "${i##*.}" == "c" ] || [ "${i##*.}" == "java" ]; then
#				echo "++++++++++++++++ $i "
				sed -i 's/\r//' $i
				sed -i 's/[ \t]*$//' $i
			fi
		fi
	done
	cd ..
}

function dos2unixf(){
	sed -i 's/\r//' $1
	sed -i 's/[ \t]*$//' $1
}

function dir2755(){
	cd $1
	for i in `ls -1`
	do
		echo $i
		if [ -d $i ]; then
			dir2755 $i
		elif [ -f $i ]; then
			chmod 755 $i;
		fi
	done
	cd ..
}

case "$1" in
	-f)
	if [ -f $2 ]; then
		dos2unixf $2
	else
		echo "Not find $2"
	fi
	;;
	-d)
	if [ -d $2 ]; then
		dos2unixd $2
	else
		echo "Not find $2"
	fi
	;;
	-a)
	PWD=$(pwd)
	dos2unixd $PWD
	;;
	-f)
	PWD=$(pwd)
	;;
	*)
	echo "Usage: $0 { -a | -d [Directory name] | -f [File name] }"
esac
