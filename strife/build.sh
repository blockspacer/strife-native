git submodule update
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && \
cd bin
./main
cd ..
