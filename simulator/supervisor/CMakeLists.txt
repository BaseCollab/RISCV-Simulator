cmake_minimum_required(VERSION 3.13)

set(SOURCES supervisor.cpp)

add_library(supervisor_impl OBJECT ${SOURCES})
target_include_directories(supervisor_impl PUBLIC ${SIMULATOR_ROOT})
