if [ "$1" = "DEBUG" ]
then
	echo "making debug build"
	mkdir build_debug; cd build_debug; export CXX="clang++-11"; export CC="clang-11"; cmake -DCMAKE_BUILD_TYPE=Debug .. ; make -j $(nproc); cd -
else
	mkdir build_release; cd build_release; export CXX="clang++-11"; export CC="clang-11"; cmake -DCMAKE_BUILD_TYPE=Release .. ; make -j $(nproc); cd -
fi
