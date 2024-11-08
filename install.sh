#!/bin/bash

BASE_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

check_executable () {
  for executable; do
    command -v $executable >/dev/null 2>&1 || {
      echo >&2 "Unable to find '$executable', make sure it is installed and is in the PATH.  Aborting."
      exit 1
    }
  done
}

check_executable cmake grep sed git

set -ex

if /sbin/ldconfig -p | grep -q libcassandra; then
    echo "Cassandra library found"
else
  echo "Cassandra library not found"
  echo "Cloning CPP Repo"
  git clone --recursive https://github.com/datastax/cpp-driver $BASE_DIR/lib/cpp-driver >/dev/null 2>&1
  echo "Compiling cpp-driver..."
  mkdir $BASE_DIR/lib/cpp-driver/build && cd $BASE_DIR/lib/cpp-driver/build
  cd $BASE_DIR/lib/cpp-driver/build && cmake -DCASS_CPP_STANDARD=17 \
        -DCASS_BUILD_STATIC=ON \
        -DCASS_BUILD_SHARED=ON \
        -DCASS_USE_TIMERFD=ON \
        -DCASS_USE_LIBSSH2=ON -DCASS_USE_ZLIB=ON \
        -DCMAKE_C_FLAGS="-fPIC" \
        -DCMAKE_CXX_FLAGS="-fPIC" \
        -Wno-error="redundant-move" \
        -DCMAKE_BUILD_TYPE="RelWithInfo" \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCASS_INSTALL_PKG_CONFIG=ON ..
  cd $BASE_DIR/lib/cpp-driver/build && make
  cd $BASE_DIR/lib/cpp-driver/build && sudo make install
  cd $BASE_DIR
  sudo rm -Rf cd $BASE_DIR/lib/cpp-driver
fi


echo "Compiling and installing the extension..."
cd $BASE_DIR/ext && phpize
# Ensure that conifgure is using latest autoconf
cd $BASE_DIR/ext && autoupdate
cd $BASE_DIR/ext && ./configure
cd $BASE_DIR/ext && make
cd $BASE_DIR/ext && sudo make install
