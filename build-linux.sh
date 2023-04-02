#!/bin/sh

if ! [ $1 ]; then
  rm -rf build
fi
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
if ! [ -e "compile_commands.json" ]; then
  ln -s build/compile_commands.json compile_commands.json
fi
cmake --build build
