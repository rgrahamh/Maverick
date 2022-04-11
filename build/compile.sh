#!/bin/bash
set -e

cd ../src/Maverick
cmake .
make
cp libmaverick* ../../bin
cd ../../bin

cd ../src/led
cmake .
make
chmod +x led
cp led ../../bin
cd ../../bin

cd ../src/red
cmake .
make
chmod +x red
cp red ../../bin
cd ../../bin
