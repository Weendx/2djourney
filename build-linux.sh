#!/bin/sh

if ! [ $1 ]; then
  rm -rf build
fi
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
