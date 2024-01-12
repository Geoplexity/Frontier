FROM ubuntu:22.04


# System setup
RUN apt-get update -y
RUN apt-get install wget -y
RUN apt-get update -y
RUN apt-get install g++ -y

# Install boost
WORKDIR /
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz
RUN echo "205666dea9f6a7cfed87c7a6dfbeb52a2c1b9de55712c9c1a87735d7181452b6 boost_1_81_0.tar.gz" | sha256sum
RUN tar -xf boost_1_81_0.tar.gz
WORKDIR /boost_1_81_0
RUN apt-get install python3.10-dev -y
RUN ./bootstrap.sh gcc
RUN apt-get install python3 python3-pip -y
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


# Install ginac
RUN apt-get install bzip2 -y
WORKDIR /
RUN mkdir ginac_and_deps
WORKDIR /ginac_and_deps
RUN wget https://www.ginac.de/CLN/cln-1.3.6.tar.bz2
RUN tar -xvf cln-1.3.6.tar.bz2
WORKDIR ./cln-1.3.6
RUN ./configure
RUN make -j 6
RUN make check
RUN make install


WORKDIR /ginac_and_deps
RUN apt install git dh-autoreconf pkg-config libreadline-dev libtool libtool-bin libbison-dev flex autogen -y
RUN wget https://www.ginac.de/ginac-1.8.7.tar.bz2
RUN tar -xvf ginac-1.8.7.tar.bz2
WORKDIR ./ginac-1.8.7
RUN autoreconf -i
RUN ./configure
RUN make -j 6
RUN make check
RUN make install

# General purpose tools
RUN apt install vim -y
RUN apt install gdb -y
