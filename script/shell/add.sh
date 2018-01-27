#!/bin/bash
a=5
b=6
echo $(($a * 3))
var=1
freq=`expr $a+$b`
echo "freq=$freq"

offset_a0=0
offset_a1=0
offset_a2=0
offset_a3=0
p=0
declare -a offset
offset=(0 1024 2048 3072)

round_add()
{
 if [ $p -ge 3 ]; then
  p=0
 else
  p=$(($p+1))
 fi
# echo "p="$p
}


round()
{
# for ((i=0; i< 4; i++));
# do
#  round_add
# done
round_add
offset_a0=${offset[$p]}
round_add
offset_a1=${offset[$p]}
round_add
offset_a2=${offset[$p]}
round_add
offset_a3=${offset[$p]}
round_add
}

show_result()
{
 echo "p:" $p "," "result:" $offset_a0 $offset_a1 $offset_a2 $offset_a3
}

for ((i=0; i<6; i++))
do
round
show_result
done
