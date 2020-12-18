#!/bin/bash

path=$(pwd)
mkdir hdttmp/
cd hdttmp
git clone https://github.com/rdfhdt/hdt-cpp/
cd hdt-cpp
cp ../../include/PlainDictionary.hpp libhdt/src/dictionary/

./gnulib.sh
./autogen.sh

./configure --prefix=$path/libhdt/
make -j
make install
cp include/PlainDictionary.hpp $path/libhdt/include/
cp libhdt/src/dictionary/FourSectionDictionary.hpp $path/libhdt/include/
cp libhdt/src/libdcs/CSD.h $path/libhdt/include/

cd ../../
rm -rf hdttmp/
