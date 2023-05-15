cmake_minimum_required(VERSION 3.22)
project(NI-PDP)
set(CMAKE_CXX_STANDARD 17)

# Sequential
add_executable(sequential sequential/sequential.cpp)
set_target_properties(sequential PROPERTIES COMPILE_FLAGS "-pedantic -Wall -Wextra -O3")
# Parallel task
add_executable(parallel_task parallel_task/parallel_task.cpp)
set_target_properties(parallel_task PROPERTIES COMPILE_FLAGS "-pedantic -Wall -Wextra -fopenmp -O3")
# Parallel data
add_executable(parallel_data parallel_data/parallel_data.cpp)
set_target_properties(parallel_data PROPERTIES COMPILE_FLAGS "-pedantic -Wall -Wextra -fopenmp -O3")
# MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
add_executable(mpi mpi/mpi.cpp)
set_target_properties(mpi PROPERTIES COMPILE_FLAGS "-pedantic -Wall -Wextra -fopenmp -O3")
target_link_libraries(mpi ${MPI_CXX_LIBRARIES})
