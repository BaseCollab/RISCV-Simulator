cmake_minimum_required(VERSION 3.13)

set(SOURCES memory_controller.cpp)

add_library(memory_impl OBJECT ${SOURCES})
target_include_directories(memory_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(simulator PUBLIC memory_impl)
