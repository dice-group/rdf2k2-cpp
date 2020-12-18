FROM ubuntu:groovy AS builder
ARG DEBIAN_FRONTEND=noninteractive
ARG CXX="clang++-11"
ARG CC="clang-11"

RUN apt-get -qq update && \
    apt-get -qq install -y make cmake clang-11 libstdc++-10-dev
RUN apt-get -qq install git

# build hdt
RUN apt-get -qq install autoconf gnulib zlib1g zlib1g-dev pkg-config libserd-0-0 libserd-dev
COPY include include
COPY generateHDTlib.sh generateHDTlib.sh
RUN sh generateHDTlib.sh

# build k2
COPY libk2 libk2
COPY CMakeLists.txt CMakeLists.txt
RUN mkdir /build
WORKDIR /build
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN make -j $(nproc)

ENTRYPOINT ["/rdf2k2"]
