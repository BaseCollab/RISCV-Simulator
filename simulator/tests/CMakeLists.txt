cmake_minimum_required(VERSION 3.13)

set(SOURCES main.cpp)

add_library(unit_tests_impl OBJECT ${SOURCES})
target_include_directories(unit_tests_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(unit_tests PUBLIC unit_tests_impl)
