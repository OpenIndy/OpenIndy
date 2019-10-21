#!/bin/sh
set -x 

cd `dirname $0`
DIR=`pwd -P`
cd $DIR

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DIR/lib ./openIndy 
