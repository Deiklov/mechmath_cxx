cmake_minimum_required(VERSION 3.17)
project(lab5)

set(CMAKE_CXX_STANDARD 11)


add_library(utf8 utf8/utf8.cpp utf8/utf8.h)

add_executable(lab5 main.cpp)

target_link_libraries(lab5 PRIVATE utf8)
