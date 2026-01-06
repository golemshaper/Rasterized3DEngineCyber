# Use an Ubuntu Base Image
FROM ubuntu:24.04 AS builder

# Set version of CMake to dowload
ARG CMAKE_VERSION=4.2.1

ENV HOME="/home/build" \
    PATH="/home/build/.local/bin:/app/cmake-${CMAKE_VERSION}-linux-x86_64/bin:${PATH}"

# Make a working directory for the app code
WORKDIR /app

# Install required tooling
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential \
        python3 \
        python3-pip \
        python3-venv \
        curl \
        nodejs \
        gcc-mingw-w64-x86-64-posix \
        g++-mingw-w64-x86-64-posix \
    && rm -rf /var/lib/apt/lists/*

# Install Cmake
RUN echo "Downloading CMake ${CMAKE_VERSION}" && \
    curl -L https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz > cmake.tar.gz && \
    tar -xf cmake.tar.gz && \
    rm cmake.tar.gz

# Create a new non-root user and group
# The useradd command with -m creates a home directory
RUN useradd -m build

# Set permissions for files/directories copied into the image
# This ensures the new user can access them
# The following assumes your application files will be in /app
RUN mkdir -p /app && \
    chown -R build:build /app && \
    chown -R build:build /home/build

# Switch to the new user for all subsequent instructions and when the container starts
USER build

# Copy config files into this current image
COPY ./CMakeLists.txt ./conanfile.txt wasm.profile requirements.txt release.py windows.profile ./

# Copy the script files
COPY ./scripts ./scripts

# Run the setup script
RUN bash ./scripts/setup.sh

# Copy the source code into this image
COPY ./SDL2GameProjectBPA ./SDL2GameProjectBPA

# Run the windows build script
RUN ./scripts/build.sh windows.profile && cp -r /app/build/bin /app/windows

# Run the wasm build script
RUN ./scripts/build.sh wasm.profile

# Start a new container image
FROM scratch

# Create a folder
WORKDIR /app

# Copy in the output of the WASM build
COPY --from=builder /app/build/bin ./

# Copy in the output of the windows build
COPY --from=builder /app/windows ./