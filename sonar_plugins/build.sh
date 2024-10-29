#!/bin/bash

mkdir build
cd build
cmake ..
cmake --build .

make package

if [[ $1 == "install" ]]; then
    sudo make install
fi
