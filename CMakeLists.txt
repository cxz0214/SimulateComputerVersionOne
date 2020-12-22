cmake_minimum_required(VERSION 3.17)
project(SimulateComputerVersionOne)

set(CMAKE_CXX_STANDARD 11)

add_executable(SimulateComputerVersionOne main.cpp Compile.cpp Compile.h SimulateComputer.h SimulateComputer.cpp Decompile.h Decompile.cpp)