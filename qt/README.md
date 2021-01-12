# Qt example

## Prerequisites

```shell
apt-get install \
  protobuf-compiler \
  ca-certificates \
  libprotobuf-dev \
  build-essential \
  libzmq3-dev \
  pkgconf \
  cmake \
  git \
  # qt
  qtbase5-dev \
  libqt5opengl5-dev \
  qt5-default
```

## Building and running

```shell
mkdir build && cd build
cmake .. && make
./demo
```
