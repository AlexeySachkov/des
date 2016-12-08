 
mkdir -p build;
cd build;

cmake -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=Debug -D CMAKE_C_COMPILER=~/Downloads/pocketcpp/MinGW/bin/gcc.exe -D CMAKE_CXX_COMPILER=~/Downloads/pocketcpp/MinGW/bin/g++.exe -D CMAKE_INSTALL_PREFIX=../install/ ..
make
make install

cd ..