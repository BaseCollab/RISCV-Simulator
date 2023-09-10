cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(RISCV-SIMULATOR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SIMULATOR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMULATOR_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(simulator simulator.cpp)

add_subdirectory(memory)
add_subdirectory(supervisor)
add_subdirectory(hart)
add_subdirectory(mmu)

