cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_EXE main.cpp)
set(GRAPH_LIB R2Graph/R2Graph.cpp)

add_library(r2graph ${GRAPH_LIB})

add_executable(lab1 ${SOURCE_EXE})
target_link_libraries(lab1 r2graph)
