ARG DEB_VERSION=13-slim
FROM debian:${DEB_VERSION} AS base

RUN apt-get update \
 && apt-get install -y --no-install-recommends ca-certificates libreproc++14 libboost-json1.83.0 clang-18 zlib1g libzstd1

FROM base AS builder
WORKDIR /build

ENV CFLAGS="-ffunction-sections -fdata-sections -O3 -flto -fPIC" \
   LDFLAGS="-flto -fPIC -s -Wl,--gc-sections" \
        CI=1

RUN mkdir /build/enum /build/cg3 /install

RUN apt-get update \
 && apt-get install -y --no-install-recommends build-essential cmake ninja-build git libclang-18-dev libreproc-dev libboost-json1.83-dev llvm-18-dev clang-18 libz-dev libzstd-dev curl

RUN cd /build/enum \
 && curl -L https://github.com/Neargye/magic_enum/releases/download/v0.9.8/magic_enum-v0.9.8.tar.gz | tar xz \
 && cmake -S . -B _build-cont -G Ninja \
 	-D CMAKE_BUILD_TYPE=Release \
 	-D MAGIC_ENUM_OPT_BUILD_TESTS=OFF \
 	-D MAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF \
 && cmake --build _build-cont \
 && cmake --install _build-cont
#     ^ this install delerately goes to system

COPY . /build/cg3

# Patch shit
RUN cd /build/cg3 \
 && sed -i "/ut CONFIG REQUIRED/d" CMakeLists.txt \
 && sed -i "60i include_directories(SYSTEM [[/usr/local/include/magic_enum]])" CMakeLists.txt \
 && sed -i "/llvm_map_components_to_libnames/d" CMakeLists.txt \
 && sed -i "140i set(llvm_libs LLVM-18)" CMakeLists.txt

RUN cd /build/cg3 \
 && cmake -S . -B _build-cont -G Ninja \
 	-D CMAKE_INSTALL_PREFIX="/opt/cg3-0.3.4" \
 	-D CMAKE_BUILD_TYPE=Release \
 && cmake --build _build-cont

RUN cd /build/cg3 \
 && DESTDIR=/install cmake --install _build-cont \
 && find /install -type f \( -name "*.hpp" -o -name "*.cmake" \) -delete \
 && find /install -type d -empty -delete \
 && find /install

FROM base

ENV PATH="$PATH:/opt/cg3-0.3.4/bin/"

COPY --from=builder /install /

