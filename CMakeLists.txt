cmake_minimum_required(VERSION 3.7)

set(app pacc-rank)
project(${app})

set(CMAKE_CXX_STANDARD 20)

file(GLOB source_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h src/*.hpp src/*.cpp src/*.c)

add_executable(${app} ${source_files})

