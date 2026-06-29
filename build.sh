#!/bin/bash

VCPKG_ROOT="${VCPKG_ROOT:-$HOME/vcpkg}"

cd cpp_files && mkdir -p cmake-build-debug && cd cmake-build-debug
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" ..
ninja
