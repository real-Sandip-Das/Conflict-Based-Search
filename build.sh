#!/bin/bash

cd cpp_files && mkdir -p cmake-build-debug && cd cmake-build-debug
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake ..
ninja
