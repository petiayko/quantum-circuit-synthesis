#!/usr/bin/env bash

rm -rf build
rm -f .tests
rm -f qcs_*

mkdir build
cd build
cmake -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .. || exit 1
cmake --build . --config Release || exit 1

cd -
cp build/tests .
cp build/qcs_* .
