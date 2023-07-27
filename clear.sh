#!/usr/bin/bash


# script to clear the files created by build
rm -r CMakeFiles ||
rm cmake_install.cmake ||
rm CMakeCache.txt ||
rm fluid-sim ||
cmake . &&
make &&
./fluid-sim