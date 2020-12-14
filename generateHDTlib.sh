#!/bin/bash

path = $(pwd)
mkdir hdttmp/
cd hdttmp
git clone https://github.com/rdfhdt/hdt-cpp/
cd hdt-cpp
./gnulib.sh
./autogen.sh

./configure --prefix=$path/libhdt/
make
make install
cp libhdt/src/dictionary/PlainDictionary.hpp $path/libhdt/include/
cp libhdt/src/dictionary/FourSectionDictionary.hpp $path/libhdt/include/
cp libhdt/src/libdcs/CSD.hpp $path/libhdt/include/

cd ../../
rm -rf hdttmp/