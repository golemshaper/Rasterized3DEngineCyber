#!/bin/bash

docker build --platform=linux/amd64 --progress=plain -t ghcr.io/golemshaper/cxx-build-tools-ubuntu-2404:latest -f Dockerfile.base .
docker build --platform=linux/amd64 --progress=plain -t golemshaper/build .
