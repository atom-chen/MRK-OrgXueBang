#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR
 
for i in `find . -name "*.png"`
do
    echo $i
      pngquant  --ext .png  --speed=1  -f $i
 
done