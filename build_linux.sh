#!/usr/bin/env bash

rm -rf build
rm -f tests
rm -f qcs_*

mkdir build
cd build
cmake -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
cmake --build . --config Release

cd -
copy build/tests .
copy build/qcs_* .
