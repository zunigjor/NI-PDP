cmake_minimum_required(VERSION 3.24)
project(NI-PDP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -fopenmp -O3")

add_executable(checkpoint_1 checkpoint_1/checkpoint_1.cpp)
add_executable(checkpoint_2 checkpoint_2/checkpoint_2.cpp)
add_executable(checkpoint_3 checkpoint_3/checkpoint_3.cpp)
