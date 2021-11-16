cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_EXE main.cpp Matrix/Matrix.cpp Matrix/Matrix.hpp)
set(GRAPH_LIB Matrix/Matrix.cpp)

add_library(Matrix ${GRAPH_LIB})

add_executable(lab3 ${SOURCE_EXE})

target_link_libraries(lab3 Matrix)
