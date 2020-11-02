#!/bin/bash


var=0

for i in `seq 1 $1`
do
    var=$(expr $var + 1)
    sleep 1
done 

echo $var




















