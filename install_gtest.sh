#!/bin/bash
cd /tmp || exit
git clone https://github.com/google/googletest
mkdir /tmp/googletest/build
cd /tmp/googletest/build || exit
cmake -DCMAKE_CXX_STANDARD=14 ..
make && sudo make install
rm -rf /tmp/googletest