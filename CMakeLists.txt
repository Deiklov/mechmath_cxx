cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_EXE main.cpp)

add_executable(lab4 ${SOURCE_EXE})

target_link_libraries(lab4 Sorts)
