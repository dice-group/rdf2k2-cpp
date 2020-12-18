#!/bin/bash

path=$(pwd)
mkdir hdttmp/
cd hdttmp
git clone https://github.com/rdfhdt/hdt-cpp/
cd hdt-cpp
cp $path/include/PlainDictionary.hpp libhdt/src/dictionary/

./gnulib.sh
./autogen.sh

./configure --prefix=$path/libhdt/
make -j
make install
cp $path/include/PlainDictionary.hpp $path/libhdt/include/
cp libhdt/src/dictionary/FourSectionDictionary.hpp $path/libhdt/include/
cp libhdt/src/libdcs/CSD.h $path/libhdt/include/
sed -i 's|#include "../libdcs/CSD.h"|#include <CSD.h>|g' $path/libhdt/include/FourSectionDictionary.hpp
sed -i 's|#include "../libdcs/CSD.h"|#include <CSD.h>|g' $path/libhdt/include/PlainDictionary.hpp

cd ../../
rm -rf hdttmp/
