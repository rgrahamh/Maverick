#!/bin/bash
set -e

cd build
./compile.sh
cd ..
if [ $# -eq 1 ]; then
	./bin/$1
fi
