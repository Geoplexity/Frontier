FROM ubuntu:22.04


# System setup
RUN apt-get update -y
RUN apt-get install wget clang -y


# Install boost
WORKDIR /
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz
RUN echo "205666dea9f6a7cfed87c7a6dfbeb52a2c1b9de55712c9c1a87735d7181452b6 boost_1_81_0.tar.gz" | sha256sum
RUN tar -xf boost_1_81_0.tar.gz
WORKDIR /boost_1_81_0
RUN apt-get install python3.10-dev -y
RUN ./bootstrap.sh clang
RUN echo "using python : 3.10 : /usr/bin/python3 ;" > ./user-config.jam
RUN ./b2 install --without-mpi --user-config="./user-config.jam"
RUN apt-get install cmake lldb -y


# Install bertini
WORKDIR /
RUN mkdir bertini_and_deps
RUN apt-get install flex bison libgmp-dev libmpfr-dev -y
RUN wget https://bertini.nd.edu/BertiniSource_v1.6.tar.gz
RUN tar -xf BertiniSource_v1.6.tar.gz
WORKDIR ./BertiniSource_v16
RUN ./configure
RUN make -j 6
RUN make install


# General purpose tools
RUN apt-get install vim -y