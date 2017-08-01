cmake_minimum_required(VERSION 3.7)
project(amplifier)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/amplifier.cpp src/amplifier.h src/lib/wire.cpp src/lib/wire.h src/parts/pt2322.cpp src/parts/pt2322.h src/parts/pt2323.cpp src/parts/pt2323.h)
add_executable(amplifier ${SOURCE_FILES})