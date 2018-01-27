#!/bin/sh

trap "echo =====Exiting=====;test_encode -A -s;exit 1" INT TERM

# typy any parm will record file instead of rtsp_server
use_rtsp=1
if [ $# -gt 1 ];then
	use_rtsp=0
fi

#set for main source buffer
x_a0=N
x_a1=E
x_a2=S
x_a3=W
x_fish=1408
x_p=0
#declare -a offset_x
#offset_x=(0, 1024, 2048, 3072)

round_add_p()
{
	if [ $x_p -ge 7 ]; then
		x_p=0
	else
		x_p=$(($x_p+1))
	fi
	#echo "x_p="$x_p
}

show_src_offset_x()
{
	echo "Result:" $x_a0 $x_a1 $x_a2 $x_a3
}

update_src_layout()
{
	case $x_p in

	0) x_a0=N; x_a1=E; x_a2=S; x_a3=W;;
	1) x_a0=E; x_a1=S; x_a2=S; x_a3=W;;
	
	2) x_a0=E; x_a1=S; x_a2=W; x_a3=N;;
	3) x_a0=S; x_a1=W; x_a2=W; x_a3=N;;
	
	4) x_a0=S; x_a1=W; x_a2=N; x_a3=E;;
	5) x_a0=W; x_a1=N; x_a2=N; x_a3=E;;
	
	6) x_a0=W; x_a1=N; x_a2=E; x_a3=S;;
	7) x_a0=N; x_a1=E; x_a2=E; x_a3=S;;

	*) x_a0=N; x_a1=E; x_a2=S; x_a3=W;;
	esac
	round_add_p
	show_src_offset_x
}

update_dewarp_view()
{
	test_dewarp -M 1 -F 185 -R 1024 --wm 1 -a0 -s1024x1024 -o 0x0 -$x_a0 -h 90 -z3/2 -m2 -a1 -s1024x1024 -o 1024x0 -$x_a1 -h 90 -z3/2 -m2 -a2 -s1024x1024 -o 2048x0 -$x_a2 -h 90 -z3/2 -m2 -a3 -s1024x1024 -o 3072x0 -$x_a3 -h 90 -z3/2 -m2 -a4 -s512x512 --ns 1520x1520 -o 4096x0 -m0 -a5 -m4 --wc-src 4 --wc-offset ${x_fish}x512  2>&1 > /dev/null
}

prepare_dewarp()
{
#init
if [ -z `lsmod |grep iav|head -1|awk '{print $1}'` ];then
	init.sh --imx178
fi

iav_state=`test_encode --show-system-state|grep 'state ='|head -1|awk '{print $4}'`
if [ $iav_state != "[encoding]" ];then
	#prevew
    test_encode -i 3072x2048 -f 20 -V480p --hdmi --mixer 0 --enc-mode 1 -X --bsize 5120x1024 --bmaxsize 5120x1024 -J --btype off -K --btype prev --bsize 480p --vout-swap 1 -w 1520 -W 2048 -P --bsize 1520x1520 --bins 1520x1520 --bino 760x300 -A --smaxsize 1920x1024

	#3A
	if [ -z `pidof test_image` ]; then
		test_image -i 0 &
	fi
	sleep 2

	#dewarp
	test_dewarp -c
	update_dewarp_view

	if [ $use_rtsp -eq 1 ]; then
		#rtsp
		if [ -z `pidof rtsp_server` ]; then
			rtsp_server &>/dev/null &
		fi
	else
		#record file
		if [ -z `pidof test_stream` ]; then
			if [ -d /mnt/media ]; then
				test_stream -f /mnt/media/bjshow &>/dev/null &
			else
				test_stream -f /mnt/bjshow &>/dev/null &
			fi
		fi
	fi

	#start encode
	sleep 2
    test_encode -A -h1920x1024 --offset 0x0 --frame-factor 30/30 -e
fi
}

stream_offset_x=0
offset_x_step=16

update_stream_fisheye()
{
	stream_offset_x=$(($stream_offset_x+$offset_x_step))
	x_fish=$(($x_fish+$offset_x_step))

	if [ $stream_offset_x -ge 2176 ]; then
		stream_offset_x=0
		x_fish=1408
	fi

	if [ $stream_offset_x -eq 0 ] || [ $stream_offset_x -eq 2048 ]; then
		update_src_layout
	fi
	test_encode -A --offset ${stream_offset_x}x0
	update_dewarp_view
	usleep 1000
}

#main
prepare_dewarp
while [ 1 ];
do
	update_stream_fisheye
done
