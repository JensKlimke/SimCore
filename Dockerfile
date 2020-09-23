FROM ubuntu:18.04
MAINTAINER Jens Klimke (jens.klimke@rwth-aachen.de)

# installation
RUN apt-get update
RUN apt-get -y install git g++ make cmake nano
RUN apt-get -y install npm nodejs
RUN apt-get -y install ruby ruby-dev
RUN apt-get -y install libgtest-dev
RUN apt-get -y install libboost-all-dev

# install gtest
RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && make install

# install ruby bundler
RUN gem install bundler

# get cucumber
RUN cd /tmp && git clone https://github.com/cucumber/cucumber-cpp.git
RUN cd /tmp/cucumber-cpp && bundle install
RUN cd /tmp/cucumber-cpp && git submodule update --init && mkdir -p build && cd build \
    && cmake -DCUKE_ENABLE_EXAMPLES=OFF -DCUKE_ENABLE_BOOST_TEST=OFF -DCUKE_ENABLE_QT=OFF -DCMAKE_INSTALL_PREFIX=/usr .. \
    && make && make test && make install

# copy code
COPY . /app


# installation
#RUN rm -rf /app/build
#RUN cd /app && mkdir build && cd build && cmake -G "Unix Makefiles" \
#    -DBUILD_TESTS=ON \
#    -DGTEST_BUILD_LIBRARY=ON \
#    -DENABLE_COVERAGE=OFF \
#    -DCREATE_DOXYGEN_TARGET=OFF

# create targets
#RUN cd /app/build && make gen_interface_unit

# make
#RUN cd /app/build && make

# documentation
#RUN cd /app/build && make test

CMD bash