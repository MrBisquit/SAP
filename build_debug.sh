#!/usr/bin/env bash
set -e

BUILD_DIR=wslbuild # Gets its own separate path

mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . -- -j$(nproc)

cd ..