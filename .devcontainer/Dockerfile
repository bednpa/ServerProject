FROM ubuntu:24.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    # Build tools
    cmake \
    ninja-build \
    clang \
    clang-format \
    clang-tidy \
    # Project dependencies
    libboost-all-dev \
    nlohmann-json3-dev \
    catch2 \
    # Debugging and analysis tools
    gdb \
    valgrind \
    # Version control
    git \
    && rm -rf /var/lib/apt/lists/*
