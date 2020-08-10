FROM ubuntu:18.04
MAINTAINER Jens Klimke (jens.klimke@rwth-aachen.de)

# installation
RUN apt-get update
RUN apt-get -y install git g++ make cmake nano
RUN apt-get -y install npm nodejs

# copy code
COPY . /app

# install gtest
RUN cd /app && bash ./install_gtest.sh

# installation
RUN rm -rf /app/build
RUN cd /app && mkdir build && cd build && cmake -G "Unix Makefiles" \
    -DBUILD_TESTS=ON \
    -DGTEST_BUILD_LIBRARY=ON \
    -DENABLE_COVERAGE=OFF \
    -DCREATE_DOXYGEN_TARGET=OFF

# create targets
RUN cd /app/build && make gen_interface_unit

# make
RUN cd /app/build && make

# documentation
RUN cd /app/build && make test

CMD bash