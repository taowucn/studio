#!/bin/bash

FIFO=/tmp/fifo
trap "exec 1000>&-; exec 1000<&-; exit 0; " 2

mkfifo $FIFO
exec 1000<>$FIFO

for((n=1;n<=3;n++))
do
  echo >&1000
done

start=$(date +%s)

for((i=1;i<=10;i++))
do
  read -u 1000
  { 
     echo ok_$i; sleep 1;
     echo >&1000
  } &
done

wait
end=$(date +%s)

exec 1000>&-
exec 1000<&-
rm -rf $FIFO
echo "start=$start, end=$end"
echo "Diff Time: `expr $end - $start`"

