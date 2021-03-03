cd /tmp
git clone --branch "release-1.10.0" --recursive https://github.com/google/googletest.git
cd googletest
cmake CMakeLists.txt
make
make install