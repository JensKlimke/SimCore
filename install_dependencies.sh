mkdir third_party
cd third_party
git clone --branch "release-1.8.0" --recursive https://github.com/google/googletest.git
cd googletest
cmake -DCMAKE_INSTALL_PREFIX=../../install CMakeLists.txt
make
make install
