cmake_minimum_required(VERSION 3.0.0)
project(Brim VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(vm)
add_subdirectory(cli)