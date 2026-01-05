#!/bin/bash
source ./.venv/bin/activate
python ./release.py WASM

## Old method
# rm -rf ./build
# conan install . -pr:b default -pr:h wasm.profile -s build_type=Release --build missing -of build
# cmake . --preset conan-release
# CXXFLAGS="-Wno-c++11-narrowing" cmake --build ./build --config Release