#!/bin/sh
set -e
export PREFIX="$HOME/opt/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"
BINUTILS=2.42
GCC=13.2.0

mkdir -p /tmp/cross && cd /tmp/cross
curl -LO https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS.tar.xz
curl -LO https://ftp.gnu.org/gnu/gcc/gcc-$GCC/gcc-$GCC.tar.xz
tar xf binutils-$BINUTILS.tar.xz && tar xf gcc-$GCC.tar.xz

mkdir build-binutils && cd build-binutils
../binutils-$BINUTILS/configure --target=$TARGET --prefix=$PREFIX \
  --with-sysroot --disable-nls --disable-werror
make -j$(nproc) && make install
cd ..

mkdir build-gcc && cd build-gcc
../gcc-$GCC/configure --target=$TARGET --prefix=$PREFIX \
  --disable-nls --enable-languages=c --without-headers
make -j$(nproc) all-gcc all-target-libgcc
make install-gcc install-target-libgcc
