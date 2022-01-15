#!/bin/bash
set -e

if [ $# -lt 1 ]; then
	printf "Please specify a program to run after compilation\n"
	exit -1
fi

cd build
./compile.sh
cd ..
./bin/$1
