cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)

set(SOURCES hart.cpp)

add_library(hart_impl OBJECT ${SOURCES})
target_include_directories(hart_impl PUBLIC ${SIMULATOR_ROOT}/common)

target_link_libraries(simulator PUBLIC hart_impl)
