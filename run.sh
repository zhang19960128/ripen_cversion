#!/bin/bash
mkdir build
cd build
cmake ..
make
cd ..
rm -rf build
./mdsimulate
