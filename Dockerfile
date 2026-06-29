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
    pkg-config \
    doxygen

RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg --depth 1 \
    && /vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/vcpkg
WORKDIR /app
COPY . /app

RUN sh build.sh

CMD ["/bin/sh", "-c", "cd cpp_files/cmake-build-debug && ./cbs_tests && ./ConflictBasedSearch"]
