#!/usr/bin/env bash

# Compiling SFML from sources

cd Submodules/SFML
git checkout 2.5.x
cd ../..

mkdir -p Submodules/SFML/Build
cd Submodules/SFML/Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j2
make install

cd ../../..

# Compiling TGUI from sources

cd Submodules/TGUI
git checkout 0.8
cd ../..

mkdir -p Submodules/TGUI/Build
cd Submodules/TGUI/Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j2
make install

cd ../../..

# Compiling golPlayground

mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make all

ls

# Moving libraries

mkdir -p Application/Submodules/
cp ../Submodules/SFML/Build/lib/* Application/Submodules/.
cp ../Submodules/TGUI/Build/lib/* Application/Submodules/.

ls

# Cleaning deployement files

rm Makefile
rm Application/Makefile
rm *.cmake
rm Application/*.cmake
rm -rf CMakeFiles
rm -rf Application/CMakeFiles
rm CMake*