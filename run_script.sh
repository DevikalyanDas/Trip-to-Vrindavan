rm -r build
mkdir build && cd build
apt-get update
apt-get install -y g++
apt-get install -y gcc
apt-get install -y cmake
apt-get install -y libpng-dev
apt-get install -y zlib1g-dev
apt-get install -y libpng-tools
cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build . --target cgray
./cgray
