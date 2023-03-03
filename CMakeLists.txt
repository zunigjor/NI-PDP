cmake_minimum_required(VERSION 3.24)
project(NI_PDP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall")

add_executable(NI_PDP main.cpp)
