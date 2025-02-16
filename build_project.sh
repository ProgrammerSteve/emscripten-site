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

# Copy the generated files to the docs folder
cd ..
mkdir -p docs
cp build/sdl_wasm_project.html docs/index.html
cp build/sdl_wasm_project.js docs/
cp build/sdl_wasm_project.wasm docs/