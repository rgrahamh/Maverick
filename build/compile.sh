#!/bin/bash
set -e

cmake .. -B.
make
cp editor ../bin
