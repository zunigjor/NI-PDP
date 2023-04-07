cmake_minimum_required(VERSION 3.24)
project(NI-PDP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -fopenmp -O3")

add_executable(sequential sequential/sequential.cpp)
add_executable(parallel_task parallel_task/parallel_task.cpp)
add_executable(parallel_data parallel_data/parallel_data.cpp)
