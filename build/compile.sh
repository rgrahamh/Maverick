#!/bin/bash
set -e

Make(){
	if [[ `uname | grep MINGW` != "" ]]; then
		cmake -G "MinGW Makefiles"
		mingw32-make.exe
	else
		cmake .
		make
	fi
}

cd ../src/Maverick
Make
cd -

cd ../src/led
Make
cd -

cd ../src/red
Make
cd -
