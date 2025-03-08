SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR

clean_cmake(){
	rm -rf CMakeFiles &>/dev/null
	rm CMakeCache.txt &>/dev/null
	rm Makefile &>/dev/null
	rm cmake_install.cmake &>/dev/null
}

cd ../src/Maverick
clean_cmake
cd - &>/dev/null

cd ../src/led
clean_cmake
cd - &>/dev/null

cd ../src/red
clean_cmake
cd - &>/dev/null

cd ../src/Cyberena
clean_cmake
cd - &>/dev/null

if [ -d ../bin ]; then
	if [[ "`ls ../bin | grep -v "Test"`" != "" ]]; then
		ls ../bin/* 2>/dev/null | grep -v "Test" | xargs rm
	fi
	if [[ "`ls ../bin/Test`" != "" ]]; then
		rm ../bin/Test/*
	fi
	echo "Cleaned out bin!"
fi
exit 0
