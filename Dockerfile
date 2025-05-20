# Dockerfile
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    libboost-program-options-dev \
    libboost-system-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy all source code
COPY . .

# Create build directory and build
RUN cmake -S . -B build && cmake --build build -- -j$(nproc)

# Set default command (optional)
CMD ["./build/bin/ClusteringRows"]
