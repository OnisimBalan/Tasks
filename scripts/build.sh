#!/usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=$(realpath "$SCRIPT_DIR/../build")

cd $BUILD_DIR
make
