cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)

set(SOURCES memory.cpp)

add_library(memory_impl OBJECT ${SOURCES})
target_include_directories(memory_impl PUBLIC ${SIMULATOR_ROOT}/common)

target_link_libraries(simulator PUBLIC memory_impl)
