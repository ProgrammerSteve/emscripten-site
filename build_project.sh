#!/bin/bash

# Source the Emscripten environment
source ~/emsdk_setup/emsdk/emsdk_env.sh

# Clean build directory
rm -rf build
mkdir -p build

# Configure with emcmake
emcmake cmake -B build -S .

# Build with emmake
cd build && emmake make