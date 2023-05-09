git submodule update --init --recursive
cd external/gtest
cmake -B ./build
cmake --build ./build --config Release
