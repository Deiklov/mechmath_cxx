cmake_minimum_required(VERSION 3.17)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_EXE main.cpp)
set(GRAPH_LIB R3Graph/R3Graph.cpp)

add_library(r3graph ${GRAPH_LIB})

add_executable(lab2 ${SOURCE_EXE})

target_link_libraries(lab2 r3graph)
