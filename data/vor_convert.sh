#!/bin/bash
for i in vor*.pov
do
  sh pov1.sh $i
  echo $i
  rm $i
done
