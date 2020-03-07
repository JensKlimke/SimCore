FROM ubuntu:18.04
MAINTAINER Jens Klimke (jens.klimke@rwth-aachen.de)

# installation
RUN apt-get update
RUN apt-get -y install git g++ make cmake nano libgtest-dev

RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && make install

# copy code
COPY . /app
RUN cd /app && git submodule update --init --recursive

# installation
RUN rm -rf /app/build
RUN cd /app && mkdir build && cd build && cmake -G "Unix Makefiles" -DBUILD_TESTS=ON -DBUILD_WEBSOCKET=OFF \
    -DBUILD_TRAFFIC_SIMULATION=ON -DBUILD_VEHICLE_MODEL=ON ..
RUN cd /app/build && make

# documentation
RUN cd /app/build && make test

CMD bash