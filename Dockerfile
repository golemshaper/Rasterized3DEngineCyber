# Use an Ubuntu Base Image
FROM ghcr.io/golemshaper/cxx-build-tools-ubuntu-2404:latest AS builder

# Copy config files into this current image
COPY ./CMakeLists.txt ./conanfile.txt wasm.profile release.py windows.profile ./

# Copy the script files
COPY ./scripts/build.sh ./scripts/build.sh

# Copy the Web assets
COPY ./web ./web

# Copy the source code into this image
COPY ./SDL2GameProjectBPA ./SDL2GameProjectBPA

# Run the windows build script
RUN ./scripts/build.sh windows.profile && cp -r /app/build/bin /app/windows

# Run the wasm build script
RUN ./scripts/build.sh wasm.profile && cp -r /app/build/bin /app/wasm

# Start a new container image
FROM scratch

# Create a folder
WORKDIR /app

# Copy in the output of the WASM build
COPY --from=builder /app/wasm /app/wasm

# Copy in the output of the windows build
COPY --from=builder /app/windows /app/windows