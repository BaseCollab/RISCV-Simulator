cmake_minimum_required(VERSION 3.13)

set(SOURCES instruction.cpp instruction_decode.cpp)

add_library(instruction_impl OBJECT ${SOURCES})
target_include_directories(instruction_impl PUBLIC ${SIMULATOR_ROOT})
