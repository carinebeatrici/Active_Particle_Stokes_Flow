#!/bin/bash
for i in *.pov
do
  sh pov1.sh $i
  echo $i
  rm $i
done
