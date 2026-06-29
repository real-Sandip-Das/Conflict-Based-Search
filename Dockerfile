FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config

RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg \
    && /vcpkg/bootstrap-vcpkg.sh

WORKDIR /app
COPY . /app

RUN cd cpp_files \
    && cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake -GNinja \
    && cmake --build build

CMD ["/bin/sh", "-c", "cd cpp_files/build && ./cbs_tests && ./ConflictBasedSearch"]
