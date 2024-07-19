#!/bin/bash

cd cpp_files && mkdir cmake-build-debug && cd cmake-build-debug
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja
