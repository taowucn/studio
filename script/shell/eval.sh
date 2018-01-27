#!/bin/bash

you=$?
echo "$?"
v1=aaa
v2=bbb
c=1
if [ $c -eq 1 ];then
vname=v$c
eval vvv="$"$vname; echo vvv:$vvv
eval vvv='$'$vname; echo vvv:$vvv
fi

vaaa="This is aaa"
eval $v1='$vaaa'; echo aaa:$aaa
eval $v1=$v1; echo aaa:$aaa
