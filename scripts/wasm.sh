rm -rf ./build
conan install . -pr:b default -pr:h wasm.profile -s build_type=Release --build missing -of build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
CXXFLAGS="-Wno-c++11-narrowing" cmake --build ./build --config Release