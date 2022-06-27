#!/bin/bash
set -e

TARGET_BUILT="false"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR

if [ $# -lt 1 ]; then
	echo "Please specify what program you'd like to compile"
	exit -1
fi

if [[ "$1" == "clean" ]]; then
	if [[ "`ls ../bin | grep -v "Test"`" != "" ]]; then
		ls ../bin/* 2>/dev/null | grep -v "Test" | xargs rm
	fi
	if [[ "`ls ../bin/Test`" != "" ]]; then
		rm ../bin/Test/*
	fi
	echo "Cleaned out bin!"
	exit 0
fi

Make(){
	if [[ `uname | grep MINGW` != "" ]]; then
		cmake -G "MinGW Makefiles"
		mingw32-make.exe
	else
		cmake .
		make
	fi
	TARGET_BUILT="true"
}

if [[ "$1" != "maverick" ]] && [[ "$1" != "led" ]] && [[ "$1" != "red" ]] && [[ "$1" != "mav_test" ]] && [[ "$1" != "cyberena" ]] && [[ "$1" != "all" ]]; then
	echo "Target $1 not found"
	exit -1
fi

#Always recompile Maverick (since everythign depends on it)
cd ../src/Maverick
Make
cd - &> /dev/null

if [[ "$1" == "led" ]] || [[ "$1" == "all" ]]; then
	cd ../src/led
	Make
	cd - &> /dev/null
fi

if [[ "$1" == "red" ]] || [[ "$1" == "all" ]]; then
	cd ../src/red
	Make
	cd - &> /dev/null
fi

#if [[ "$1" == "cyberena" ]] || [[ "$1" == "all" ]]; then
#	cd ../src/Cyberena
#	Make
#	cd - &> /dev/null
#fi

if [[ "$1" == "mav_test" ]] || [[ "$1" == "all" ]]; then
	cd ../src/Test/Maverick
	Make
	cd - &> /dev/null
fi

exit 0
