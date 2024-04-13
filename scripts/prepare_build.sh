#!/usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=$(realpath "$SCRIPT_DIR/../build")

echo $BUILD_DIR
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

cmake -S ../source -B . -G"Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

