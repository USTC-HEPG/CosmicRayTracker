#!/bin/bash
cd ..
mkdir build
cd build
cmake -DGeant4_DIR=$MYGEANT2 -G "Eclipse CDT4 - Unix Makefiles" ../B4
