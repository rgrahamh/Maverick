if [ $# -lt 1 ]; then
	printf "Please specify a program to run after compilation"
	exit -1
fi

cd build
./compile.sh
cd ..
./bin/$1
