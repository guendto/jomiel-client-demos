# C example

## Prerequisites

```shell
apt-get install \
    protobuf-c-compiler \
    libprotobuf-c-dev \
    libprotobuf-dev \
    build-essential \
    libczmq-dev \
    pkgconf \
    cmake
```

## Building and running

```shell
mkdir build && cd build
cmake .. && make
./demo [args...]
```
