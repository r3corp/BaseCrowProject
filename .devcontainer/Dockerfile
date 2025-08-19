#FROM nvidia/cuda:12.2.2-devel-ubuntu22.04
FROM ubuntu:latest

RUN apt-get update && apt-get install -y apt-utils pkg-config && apt-get install -y cmake build-essential git libasio-dev curl libcurl4-openssl-dev meson libbrotli-dev zlib1g zlib1g-dev libboost-all-dev

RUN git clone https://github.com/CrowCpp/Crow

WORKDIR /Crow/build

RUN cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROWBUILD_TESTS=OFF && make -j12 && make install

WORKDIR /

RUN git clone https://github.com/pistacheio/pistache

WORKDIR /pistache

RUN meson setup build                           \
--buildtype=release                             \
-DPISTACHE_USE_SSL=true                         \
-DPISTACHE_BUILD_EXAMPLES=true                  \
-DPISTACHE_BUILD_TESTS=true                     \
-DPISTACHE_BUILD_DOCS=false                     \
-DPISTACHE_USE_CONTENT_ENCODING_BROTLI=true     \
-DPISTACHE_USE_CONTENT_ENCODING_DEFLATE=true    \
--prefix="$PWD/prefix" && meson compile -C build && meson install -C build

#### GRPC
#WORKDIR /deps
#RUN git clone --recurse-submodules -b v1.64.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
#RUN mkdir -p /deps/grpc/build && cd /deps/grpc/build && \
#    cmake -DgRPC_INSTALL=ON \
#        -DgRPC_BUILD_TESTS=OFF .. && \
#        make -j8 install

WORKDIR /

RUN git clone https://github.com/r3corp/BaseCrowProject

WORKDIR /BaseCrowProject