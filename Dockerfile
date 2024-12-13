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
    libglm-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN mkdir -p build && \
    cd build && \
    cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. && \
    make -j$(nproc) VERBOSE=1 && \
    ls -l /app/build && \
    chmod +x /app/build/cute-gis

CMD ["./build/cute-gis"]
