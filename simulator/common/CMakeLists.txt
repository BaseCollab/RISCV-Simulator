cmake_minimum_required(VERSION 3.13)

set(SOURCES utils/bit_ops.cpp)

add_library(common_impl OBJECT ${SOURCES})
target_include_directories(common_impl PUBLIC ${SIMULATOR_ROOT})
