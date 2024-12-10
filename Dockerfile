FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    libgtest-dev \
    libcurl4-openssl-dev \
    libgdal-dev \
    libglew-dev \
    libglfw3-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN [ ! -d build ] && mkdir build || true && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make clean && \
    rm -rf /var/lib/apt/lists/*


CMD ["./build/cute-gis"]
